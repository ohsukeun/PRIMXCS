package egovframework.com.primx.fun.dpt.service;

import java.io.Serializable;
import java.util.Date;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * @Class Name : DeptVO.java
 * @Description : 부서 정보 테이블 (COMTNDEPTINFOMGR)
 * <pre>
 * 주요 컬럼:
 * - DEPT_SEQ : ID
 * - COMPANY_ID : 회사 ID
 * - DEPT_ID : 부서 ID
 * - DEPT_NAME : 부서 이름
 * - PARENT_ID : 부모 부서 ID
 * - INSERTED_BY_HR_YN : 인사연동으로 생성됨 여부
 * - LEVEL : 레벨
 * - SORT_ORDER : 정렬
 * - CREATED_DATE : 생성 일자
 * </pre>
 * @Modification Information
 *
 *<pre>
 * << 개정이력(Modification Information) >>
 *
 *   수정일              수정자          수정내용
 *   ----------       --------  ---------------------------
 *   2025.01.06        김영우     최초 생성
 *</pre>
 *
 *  @author 김영우
 *  @since 2025.01.06
 *  @version 1.0
 *  @see
 *  
 */
/**
 * @author 김영우
 *
 */
@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class DeptVO implements Serializable {
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
