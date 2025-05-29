package egovframework.com.primx.fun.cmm.service.impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;

import org.springframework.stereotype.Service;

import egovframework.com.cmm.LoginVO;
import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.primx.fun.cmm.service.AlarmVO;
import egovframework.com.primx.fun.cmm.service.PrimxAlarmService;

@Service("primxAlarmService")
public class PrimxAlarmServiceImpl implements PrimxAlarmService {
    
    @Resource(name = "alarmDAO")
    private AlarmDAO alarmDAO;

    @Override
    public Map<String, Object> selectAlarmList() throws Exception {
        Map<String, Object> response = new HashMap<>();

        // 사용자 정보를 가져온다
        LoginVO user = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
        String userId = user.getId();
        
        // 사용자의 권한을 가져온다
        List<String> authList = EgovUserDetailsHelper.getAuthorities();
        String userAuth = authList.get(0);
        String userRole = "";
        // userAut 에 ADMIN 이 포함되어 있으면 ADMIN 으로 설정
        if(userAuth.contains("ADMIN")){
            userRole = "ADMIN";
        } else {
            userRole = "USER";
        }

        List<AlarmVO> alarmList = alarmDAO.selectAlarmList(userId, userRole);
        int totalCount = alarmDAO.selectAlarmListCount(userId, userRole);
        
        response.put("alarmList", alarmList);
        response.put("totalCount", totalCount);
        response.put("status", "success");
        response.put("message", "success");
        return response;
    }

    @Override
    public Map<String, Object> readAlarm(String seqNo) throws Exception {
        Map<String, Object> response = new HashMap<>();

        alarmDAO.readAlarm(seqNo);
        response.put("status", "success");
        response.put("message", "success");
        return response;
    }

    @Override
    public Map<String, Object> readAllAlarm() throws Exception {
        Map<String, Object> response = new HashMap<>();
        
        // 사용자 정보를 가져온다
        LoginVO user = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
        String userId = user.getId();

        alarmDAO.readAllAlarm(userId);

        response.put("status", "success");
        response.put("message", "success");
        return response;
    }
}
