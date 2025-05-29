package egovframework.com.primx.fun.cmm.service;

import java.util.Date;

import lombok.Data;

@Data
public class DeptVO {
    private static final long serialVersionUID = 1L;

    /** ID */
    private long deptSeq;

    /** 회사 ID */
    private String companyId;

    /** 부서 ID */
    private String deptId;

    /** 부서 이름 */
    private String deptName;
    
    /** 인사연동으로 생성됨 여부 */
    private String insertedByHrYn;

    /** 부모 부서 ID */
    private String parentId;

    /** 레벨 */
    private Long level;

    /** 부서 위치 이동 */
    private Long sortOrder;
    
    /** 생성 일자 */
    private Date createdDate;

    /** 수정 일자 */
    private Date updateDate;

    /** 사용여부 */
    private String useYn;
}