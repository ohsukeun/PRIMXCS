package egovframework.com.primx.fun.cmm.service;

import lombok.Data;

@Data
public class DeptSearchVO {
	private static final long serialVersionUID = 1L;
	private String searchType;      // 검색 조건 (부서명/사용자명)
	private String searchKeyword;   // 검색어
	private String companyId;      // 회사 ID
	private String deptId;         // 부서 ID
	private String parentId;       // 상위 부서 ID
	private String insertedByHrYn; // 인사연동 여부
	private boolean includeSubDept; // 하위부서 포함 여부
	private String userType;       // 사용자 구분 (전체/관리자/사용자)
	private String syncType;       // 구분 (전체/연동/수동)
}
