package egovframework.com.primx.fun.cmm.service.impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.cmm.service.AlarmMapVO;
import egovframework.com.primx.fun.cmm.service.AlarmVO;

@Repository("alarmDAO")
public class AlarmDAO extends EgovComAbstractDAO {
    
    public List<AlarmVO> selectAlarmList(String targetId, String userRole) throws Exception {
        Map<String, Object> param = new HashMap<>();
        param.put("targetId", targetId);
        param.put("userRole", userRole);
        return selectList("alarmDAO.selectAlarmList", param);
    }

    public int selectAlarmListCount(String targetId, String userRole) throws Exception {
        Map<String, Object> param = new HashMap<>();
        param.put("targetId", targetId);
        param.put("userRole", userRole);
        return selectOne("alarmDAO.selectAlarmListCount", param);
    }

    public int insertAlarm(AlarmVO alarmVO) throws Exception {
        return insert("alarmDAO.insertAlarm", alarmVO);
    }

    public int updateAlarm(AlarmVO alarmVO) throws Exception {
        return update("alarmDAO.updateAlarm", alarmVO);
    }

    public List<AlarmMapVO> selectAlarmMapList() throws Exception {
        return selectList("alarmDAO.selectAlarmMapList");
    }

    public int selectAlarmMapListCount() throws Exception {
        return selectOne("alarmDAO.selectAlarmMapListCount");
    }

    public List<AlarmMapVO> selectAlarmMapListByCode(String code) throws Exception {
        Map<String, Object> param = new HashMap<>();
        param.put("code", code);
        return selectList("alarmDAO.selectAlarmMapListByCode", param);
    }

    public int readAlarm(String seqNo) throws Exception {
        Map<String, Object> param = new HashMap<>();
        param.put("seqNo", seqNo);
        return update("alarmDAO.readAlarm", param);
    }

    public int readAllAlarm(String targetId) throws Exception {
        Map<String, Object> param = new HashMap<>();
        param.put("targetId", targetId);
        return update("alarmDAO.readAllAlarm", param);
    }
    
}