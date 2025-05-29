package egovframework.com.primx.fun.cmm.service.impl;

import java.util.HashMap;
import java.util.Map;

import javax.annotation.Resource;

import org.springframework.stereotype.Service;

import egovframework.com.primx.fun.cmm.service.MyPageUpdateDTO;
import egovframework.com.primx.fun.cmm.service.PrimxCommonService;
import egovframework.com.primx.fun.cmm.service.UserInfoVO;
import egovframework.com.utl.sim.service.EgovFileScrty;

@Service("PrimxCommonService")
public class PrimxCommonServiceImpl implements PrimxCommonService {

    @Resource(name = "commonDAO")
    private CommonDAO commonDAO;

    @Override
    public UserInfoVO getMyPageInfo(String userId) throws Exception {
        return commonDAO.selectUserInfo(userId);
    }

    @Override
    public Map<String, Object> updateMyPageInfo(MyPageUpdateDTO updateDTO) throws Exception {
        Map<String, Object> response = new HashMap<>();
        
        // 비밀번호 변경이 있는 경우
        if (updateDTO.getPassword() != null && !updateDTO.getPassword().isEmpty()) {
            // 기존 비밀번호 확인
            if (updateDTO.getCurrentPassword() == null || updateDTO.getCurrentPassword().isEmpty()) {
                response.put("success", false);
                response.put("message", "현재 비밀번호를 입력해주세요.");
                return response;
            }

            // 현재 비밀번호 검증
            UserInfoVO userInfo = commonDAO.selectUserInfo(updateDTO.getUserId());
            String currentEncryptedPassword = EgovFileScrty.encryptPassword(updateDTO.getCurrentPassword(), updateDTO.getUserId());
            if (!currentEncryptedPassword.equals(userInfo.getPassword())) {
                response.put("success", false);
                response.put("message", "현재 비밀번호가 일치하지 않습니다.");
                return response;
            }

            // 새 비밀번호 확인
            if (!updateDTO.getPassword().equals(updateDTO.getConfirmPassword())) {
                response.put("success", false);
                response.put("message", "새 비밀번호가 일치하지 않습니다.");
                return response;
            }

            // 새 비밀번호 암호화
            String newpassword = updateDTO.getPassword();
            String enpassword = EgovFileScrty.encryptPassword(newpassword, updateDTO.getUserId());
            updateDTO.setPassword(enpassword);
        }
        
        // 회원 정보 업데이트
        commonDAO.updateUserInfo(updateDTO);
        
        response.put("success", true);
        response.put("message", "회원 정보가 수정되었습니다.");
        return response;
    }
}
