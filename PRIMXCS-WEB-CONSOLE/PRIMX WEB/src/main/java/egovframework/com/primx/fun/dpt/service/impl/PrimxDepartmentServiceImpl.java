package egovframework.com.primx.fun.dpt.service.impl;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;
import javax.transaction.Transactional;

import org.springframework.stereotype.Service;

import egovframework.com.primx.fun.cmm.service.MessageException;
import egovframework.com.primx.fun.dpt.service.CompanyDTO;
import egovframework.com.primx.fun.dpt.service.PrimxDepartmentService;
import egovframework.com.primx.fun.pol.service.ReleasePolicyVO;
import egovframework.com.primx.fun.pol.service.impl.PolicyDAO;
import egovframework.com.primx.fun.usr.service.impl.UserDAO;
import egovframework.com.primx.fun.dpt.service.DeptSearchVO;
import egovframework.com.primx.fun.dpt.service.DeptVO;

@Service("primxDepartmentService")
public class PrimxDepartmentServiceImpl implements PrimxDepartmentService {

	@Resource(name = "departmentDAO")
	private DepartmentDAO departmentDAO;

	@Resource(name = "userDAO")
	private UserDAO userDAO;

	@Resource(name = "policyDAO")
	private PolicyDAO policyDAO;
	
	@Override
	@Transactional
	public DeptVO insertDepartment(DeptVO deptVO, String deptCode) throws Exception {

		int insertCnt = 0;

		Long lastOrderIndex = departmentDAO.selectOrderIndex();
		String company = (deptVO.getCompanyId() != null) ? deptVO.getCompanyId() : "00000001";
		DeptVO department = DeptVO.builder()
			.deptName(deptVO.getDeptName())
			.deptId(deptCode)
			.parentId(deptVO.getParentId())
			.insertedByHrYn("N")
			.companyId(company)
			.level(1L)
			.createdDate(new Date())
			.sortOrder(Long.valueOf(lastOrderIndex.longValue() + 1L))
			.build();
		departmentDAO.insertDepartment(department);

		// 부서 경로 조회
		String deptPath = departmentDAO.selectDeptPath(deptCode);
		// 부서경로가 안나오는 경우 회사 경로를 확인한다
		if(deptPath == null) {
			deptPath = departmentDAO.selectCompanyPath(deptCode);
		}
		
		// 등록
		insertCnt = policyDAO.insertDistributionPolicy(deptCode, null, "9999-12-31", deptPath, "N");

		return department;
	}

	@Override
	@Transactional
	public DeptVO updateDepartment(DeptVO deptVO) throws Exception {
		// 부서 정보 조회 (HR 연동 여부 체크 없이)
		DeptVO existingDept = departmentDAO.selectDeptId(deptVO.getDeptId());
		
		if (existingDept == null) {
			throw new MessageException("DEPT_NOT_FOUND", "message.dept.notfound");
		}
		
		// HR 연동 부서 체크
		if ("Y".equals(existingDept.getInsertedByHrYn())) {
			throw new MessageException("DEPT_HR_LINKED", "message.dept.hr.noupdatendelete");
		}
		
		int result = departmentDAO.updateDeptName(deptVO.getDeptId(), deptVO.getDeptName());

		DeptSearchVO searchVO = DeptSearchVO.builder()
			.includeSubDept(true)
			.searchKeyword(deptVO.getDeptName())
			.build();

		// 하위부서를 포함한 부서 목록 조회
		List<DeptVO> deptList = departmentDAO.selectDeptTree(searchVO);
		
		// 하위부서들은 모두 부서 경로를 업데이트
		for (DeptVO dept : deptList) {
			String deptPath = departmentDAO.selectDeptPath(dept.getDeptId());

			// 부서경로가 안나오는 경우 회사 경로를 확인한다
			if(deptPath == null) {
				deptPath = departmentDAO.selectCompanyPath(dept.getDeptId());
			}

			ReleasePolicyVO releasePolicy = policyDAO.selectDistributionPolicyByDeptId(dept.getDeptId());
			if(releasePolicy != null) {
				// 기존 정책 시작 종료 기간 변경
				policyDAO.updateDeptPathDistributionPolicy(releasePolicy.getRegId(), deptPath);
			}
			else {
				// 등록
				policyDAO.insertDistributionPolicy(dept.getDeptId(), null, "9999-12-31", deptPath, "N");
			}
		}
		
		if (result > 0) {
			existingDept.setDeptName(deptVO.getDeptName());
			return existingDept;
		} else {
			throw new MessageException("DEPT_UPDATE_FAIL", "message.dept.update.fail");
		}
	}

	@Override
	public DeptVO selectDepartmentByDeptId(String deptId) throws Exception {
		DeptVO deptVO = departmentDAO.selectDeptId(deptId);

		if (deptVO == null) {
			throw new MessageException("DEPT_NOT_FOUND", "message.dept.notfound");
		}
		return deptVO;
	}

	@Override
	public Map<String, Object> selectDeptTree(DeptSearchVO searchVO) throws Exception {
		List<DeptVO> result = departmentDAO.selectDeptTree(searchVO);
		
		Map<String, Object> response = new HashMap<>();
		response.put("status", "success");
		response.put("data", result);
		
		return response;
	}

	@Override
	public String deleteDepartment(String deptId) throws Exception {
		// 부서 정보 조회
		DeptVO deptVO = departmentDAO.selectDeptId(deptId);
		
		if (deptVO == null) {
			throw new MessageException("DEPT_NOT_FOUND", "message.dept.notfound");
		}
		
		// HR 연동 부서 체크
		if ("Y".equals(deptVO.getInsertedByHrYn())) {
			throw new MessageException("DEPT_HR_LINKED", "message.dept.hr.noupdatendelete");
		}
		
		// 하위 부서 존재 여부 체크
		List<DeptVO> childDepts = departmentDAO.selectParentId(deptId);
		if (!childDepts.isEmpty()) {
			throw new MessageException("DEPT_HAS_CHILDREN", "message.dept.haschildren");
		}
		
		// 해당 부서에 속한 사용자들의 부서를 NO_DEPT로 변경
		departmentDAO.updateUsersDeptToNoDept(deptId);
		
		// 부서 삭제
		int result = departmentDAO.deleteDepartment(deptId);

		ReleasePolicyVO releasePolicy = policyDAO.selectDistributionPolicyByDeptId(deptId);
		if(releasePolicy != null) {
			policyDAO.deleteDistributionPolicy(releasePolicy.getRegId());
		}
		
		if (result > 0) {
			return "message.dept.delete.success";
		} else {
			throw new MessageException("DEPT_DELETE_FAIL", "message.dept.delete.fail");
		}
	}

	@Override
	public Map<String, Object> selectCompanyList() throws Exception {
		List<CompanyDTO> result = departmentDAO.selectCompanyList();
		Map<String, Object> response = new HashMap<>();
		response.put("status", "success");
		response.put("data", result);
		return response;
	}

	
			// 선택된 deptId로 검색
			// 선택된 deptId와 parentId가 deptId인 부서 검색

	@Override
	public List<String> selectDeptUserIdList(DeptSearchVO searchVO) throws Exception {
		List<DeptVO> result = departmentDAO.selectDeptTreeByDeptId(searchVO);
		List<String> deptIds = new ArrayList<>();
		for (DeptVO dept : result) {
			System.out.println("############################ deptId : " + dept.getDeptId());
			deptIds.add(dept.getDeptId());
		}
		System.out.println("#####################END END END END END END END END END END END#######");
		return deptIds;
	}

	@Override
	public Map<String, Object> checkCompanyCode(String companyCode) throws Exception {
		Map<String, Object> response = new HashMap<>();
		int result = departmentDAO.selectCompanyCode(companyCode);	
		if (result > 0) {
			response.put("success", true);
			response.put("message", "exist");
		} else {
			response.put("success", false);
			response.put("message", "not exist");
		}
		return response;
	}

	@Override
	public Map<String, Object> loadRootCompanyInfo() throws Exception {
		Map<String, Object> response = new HashMap<>();
		DeptVO result = departmentDAO.selectRootCompanyInfo();

		if (result == null) {
			throw new MessageException("DEPT_NOT_FOUND", "message.dept.notfound");
		}
		
		response.put("success", true);
		response.put("data", result);
		return response;
	}

	@Override
	public Map<String, Object> loadCompanyCodeList() throws Exception {
		List<CompanyDTO> result = departmentDAO.loadCompanyCodeList();
		Map<String, Object> response = new HashMap<>();
		response.put("status", "success");
		response.put("data", result);
		return response;
	}
}