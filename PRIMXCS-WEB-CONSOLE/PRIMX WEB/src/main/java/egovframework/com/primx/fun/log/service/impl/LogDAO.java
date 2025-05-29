package egovframework.com.primx.fun.log.service.impl;

import java.util.List;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.log.service.AgentProductLogDTO;
import egovframework.com.primx.fun.log.service.ManageLogEntityDTO;
import egovframework.com.primx.fun.log.service.AgentProductsSearchVO;
import egovframework.com.primx.fun.log.service.ManageLogSearchVO;

@Repository("logDAO")
public class LogDAO extends EgovComAbstractDAO {


	public List<ManageLogEntityDTO> selectListManageLog(ManageLogSearchVO searchVO) {
		return selectList("logDAO.selectListManageLog", searchVO);
	}

	public int selectListManageLogCnt(ManageLogSearchVO searchVO) {
		return (Integer)selectOne("logDAO.selectListManageLogCnt", searchVO);
	}

	public List<AgentProductLogDTO> selectListAgentProductsLog(AgentProductsSearchVO searchVO) {
		return selectList("logDAO.selectListAgentProductsLog", searchVO);
	}

	public int selectListAgentProductsLogCnt(AgentProductsSearchVO searchVO) {
		return (Integer)selectOne("logDAO.selectListAgentProductsLogCnt", searchVO);
	}

	public List<AgentProductLogDTO> selectListProductLog(String userId) {
		return selectList("logDAO.selectListProductLog", userId);
	}
}
