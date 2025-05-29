package egovframework.com.primx.fun.cmm.service.impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;

import org.springframework.stereotype.Service;

import egovframework.com.primx.fun.cmm.service.DeptSearchVO;
import egovframework.com.primx.fun.cmm.service.DeptVO;
import egovframework.com.primx.fun.cmm.service.PrimxAdminService;

@Service("primxAdminService")
public class PrimxAdminServiceImpl implements PrimxAdminService {

	@Resource(name = "commonDAO")
	private CommonDAO commonDAO;

	@Override
	public Map<String, Object> selectDeptTree(DeptSearchVO searchVO) throws Exception {
		List<DeptVO> result = commonDAO.selectDeptTree(searchVO);
		
		Map<String, Object> response = new HashMap<>();
		response.put("status", "success");
		response.put("data", result);
		
		return response;
	}

}
