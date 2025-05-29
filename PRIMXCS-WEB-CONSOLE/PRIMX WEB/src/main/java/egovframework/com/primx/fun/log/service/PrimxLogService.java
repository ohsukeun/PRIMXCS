package egovframework.com.primx.fun.log.service;

import java.util.Map;

public interface PrimxLogService {

	public Map<String, Object> selectListManageLog(ManageLogSearchVO searchVO) throws Exception;
	public Map<String, Object> selectListProductLog(AgentProductsSearchVO searchVO) throws Exception;

}
