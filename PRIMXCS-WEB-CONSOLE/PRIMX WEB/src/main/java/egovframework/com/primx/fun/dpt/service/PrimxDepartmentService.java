package egovframework.com.primx.fun.dpt.service;

import java.util.List;
import java.util.Map;

public interface PrimxDepartmentService {

	public DeptVO insertDepartment(DeptVO deptVO, String deptCode) throws Exception;
	public DeptVO updateDepartment(DeptVO deptVO) throws Exception;
	public DeptVO selectDepartmentByDeptId(String deptId) throws Exception;
	public Map<String, Object> selectDeptTree(DeptSearchVO searchVO) throws Exception;
	public String deleteDepartment(String deptId) throws Exception;
	public Map<String, Object> selectCompanyList() throws Exception;
	public List<String> selectDeptUserIdList(DeptSearchVO searchVO) throws Exception;
	public Map<String, Object> checkCompanyCode(String companyCode) throws Exception;
	public Map<String, Object> loadRootCompanyInfo() throws Exception;
	public Map<String, Object> loadCompanyCodeList() throws Exception;
}
