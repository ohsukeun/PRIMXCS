package egovframework.com.primx.fun.cmm.service;

import java.util.Map;

public interface PrimxAlarmService {
    Map<String, Object> selectAlarmList() throws Exception;
    Map<String, Object> readAlarm(String seqNo) throws Exception;
    Map<String, Object> readAllAlarm() throws Exception;
}
