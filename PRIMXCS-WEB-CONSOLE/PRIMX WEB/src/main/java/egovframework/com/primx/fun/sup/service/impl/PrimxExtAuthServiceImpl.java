package egovframework.com.primx.fun.sup.service.impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;
import javax.transaction.Transactional;

import org.springframework.stereotype.Service;

import egovframework.com.primx.fun.sup.service.PrimxExtAuthService;
import egovframework.com.primx.fun.sup.service.SmartContractDTO;

@Service("primxExtAuthService")
public class PrimxExtAuthServiceImpl implements PrimxExtAuthService {

    @Resource(name = "extAuthDAO")
    private ExtAuthDAO extAuthDAO;

    @Override
    @Transactional
    public Map<String, Object> applyXZEDSmartContractSetting(List<SmartContractDTO> smartContractDTOList) throws Exception {
        
        // 사용자 리스트 로그 출력
        System.out.println("등록된 사용자 리스트: " + smartContractDTOList);

        Map<String, Object> result = new HashMap<>();

        for(SmartContractDTO smartContractDTO : smartContractDTOList){
            
            int insertCount = extAuthDAO.applyXZEDSmartContractSetting(smartContractDTO);
            if(insertCount <= 0){
                throw new Exception("스마트 컨트랙트 설정에 실패했습니다.");
            }
        }
        result.put("result", "success");
        result.put("message", "스마트 컨트랙트 설정이 적용되었습니다.");
        return result;
    }

    @Override
    public Map<String, Object> checkSmartContractSetting(SmartContractDTO smartContractDTO) {
        Map<String, Object> result = new HashMap<>();
        SmartContractDTO checkSmartContractDTO = extAuthDAO.checkSmartContractSetting(smartContractDTO);
        if(checkSmartContractDTO != null){
            result.put("result", "success");
        } else {
            result.put("result", "fail");
        }
        return result;
    }

} 