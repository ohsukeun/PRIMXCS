package egovframework.com.primx.fun.sys.service;

import java.io.Serializable;

import lombok.Data;

/**
 * @Class Name : ProductsVO.java
 * @Description : 제품 테이블 (T_PRODUCTS)
 * <pre>
 * 주요 컬럼:
 * - id : ID
 * - product_name : 제품명
 * </pre>
 * @Modification Information
 *
 *<pre>
 * << 개정이력(Modification Information) >>
 *
 *   수정일              수정자          수정내용
 *   ----------       --------  ---------------------------
 *   2024.12.03        김영우     최초 생성
 *</pre>
 *
 *  @author 김영우
 *  @since 2024.12.03
 *  @version 1.0
 *  @see
 *  
 */
@Data
public class ProductsVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private Long id;

    /** 제품명 */
    private String productName;
    
}
