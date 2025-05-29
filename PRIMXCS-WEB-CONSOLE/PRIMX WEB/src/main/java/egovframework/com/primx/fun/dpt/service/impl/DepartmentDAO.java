package egovframework.com.primx.fun.dpt.service.impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.dpt.service.CompanyDTO;
import egovframework.com.primx.fun.dpt.service.DeptSearchVO;
import egovframework.com.primx.fun.dpt.service.DeptVO;

@Repository("departmentDAO")
public class DepartmentDAO extends EgovComAbstractDAO {

	public DeptVO selectDeptId(String deptId) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptId", deptId);

		return selectOne("departmentDAO.selectDeptId", paramMap);
	}

	public List<DeptVO> selectParentId(String parentId) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("parentId", parentId);

		return selectList("departmentDAO.selectParentId", paramMap);
	}

	public Long selectOrderIndex() {
		return (Long)selectOne("departmentDAO.selectOrderIndex");
	}

	int selectCountByDeptId(String deptId) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptId", deptId);

		return selectOne("departmentDAO.selectCountByDeptId", paramMap);
	}

//	@Query(value = "update T_TEMP_DEPT_MASTER set C_COMPANY_ID = :companyId", nativeQuery = true)
	public int  updateTempDeptMasterCompanyId(String companyId) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("companyId", companyId);

		return update("departmentDAO.updateTempDeptMasterCompanyId", paramMap);
	}

//	@Query(value = "update T_DEPT_MASTER set C_COMPANY_ID = :companyId", nativeQuery = true)
	public int  updateDeptMasterCompanyId(String companyId, String preCompanyId) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("companyId", companyId);
		paramMap.put("preCompanyId", preCompanyId);

		return update("departmentDAO.updateDeptMasterCompanyId", paramMap);
	}
	
	public int updateDepartment(DeptVO deptVO) {
		return (Integer)update("departmentDAO.updateDepartment",deptVO);
	}

	public int updateParentId(Long seqNo, String parentId) {
		
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("seqNo", seqNo);
		paramMap.put("parentId", parentId);

		return (Integer)update("departmentDAO.updateParentId",paramMap);
	}
	
	public int deleteById(Long seqNo) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("seqNo", seqNo);

		return (Integer)update("departmentDAO.deleteById",paramMap);
	}

	public List<DeptVO> selectDeptTree(DeptSearchVO searchVO) {
		return selectList("departmentDAO.selectDeptTree", searchVO);
	}

	public DeptVO insertDepartment(DeptVO deptVO) {
		insert("departmentDAO.insertDepartment", deptVO);
		return deptVO;
	}

	public int updateDeptName(String deptId, String deptName) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptId", deptId);
		paramMap.put("deptName", deptName);
		
		return update("departmentDAO.updateDeptName", paramMap);
	}

	public int updateChildrenParentId(String deptId, String newParentId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptId", deptId);
		paramMap.put("newParentId", newParentId);
		
		return update("departmentDAO.updateChildrenParentId", paramMap);
	}

	public int updateUsersDeptToNoDept(String deptId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptId", deptId);
		
		return update("departmentDAO.updateUsersDeptToNoDept", paramMap);
	}

	public int deleteDepartment(String deptId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptId", deptId);
		
		return delete("departmentDAO.deleteDepartment", paramMap);
	}

	public List<CompanyDTO> selectCompanyList() {
		return selectList("departmentDAO.selectCompanyList");
	}

	public List<DeptVO> selectDeptUserIdList(DeptSearchVO searchVO) {
		return selectList("departmentDAO.selectDeptUserIdList", searchVO);
	}

	public String selectDeptName(String deptId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptId", deptId);

		return selectOne("departmentDAO.selectDeptName", paramMap);
	}
	
	public List<DeptVO> selectDeptTreeByDeptId(DeptSearchVO searchVO) {
		return selectList("departmentDAO.selectDeptTreeByDeptId", searchVO);
	}

	public int selectCompanyCode(String companyCode) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("companyCode", companyCode);

		return selectOne("departmentDAO.selectCompanyCode", paramMap);
	}

	public DeptVO selectRootCompanyInfo() {
		return selectOne("departmentDAO.selectRootCompanyInfo");
	}

	public String selectDeptPath(String deptId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptId", deptId);
		return selectOne("departmentDAO.selectDeptPath", paramMap);
	}

	public String selectCompanyPath(String companyId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("companyId", companyId);
		return selectOne("departmentDAO.selectCompanyPath", paramMap);
	}

	public List<CompanyDTO> loadCompanyCodeList() {
		return selectList("departmentDAO.loadCompanyCodeList");
	}

	public List<DeptVO> selectIncludeSubDeptByDeptId(String deptId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptId", deptId);
		return selectList("departmentDAO.selectIncludeSubDeptByDeptId", paramMap);
	}
}