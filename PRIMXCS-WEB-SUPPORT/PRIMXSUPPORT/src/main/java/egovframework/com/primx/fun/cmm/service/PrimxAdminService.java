package egovframework.com.primx.fun.cmm.service;

import java.util.Map;

import org.springframework.web.bind.annotation.ModelAttribute;

public interface PrimxAdminService {

	public Map<String, Object> selectDeptTree(@ModelAttribute("searchVO") DeptSearchVO searchVO) throws Exception;
}
