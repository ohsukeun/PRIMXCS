package egovframework.com.primx.fun.pol.service;

import java.io.Serializable;

import egovframework.com.cmm.ComDefaultVO;
import egovframework.com.primx.fun.cmm.util.DateUtil;
import egovframework.com.primx.fun.sys.service.ProductsVO;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class PolicyVO extends ComDefaultVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private Long id;

    /** 정책 코드 */
    private String policyCode;

    /** 정책명 */
    private String policyName;

    /** 정책 JSON */
    private String policyJson;

    /** 정책 XML */
    private byte[] policyXml;

    /** 제품 ID */
    private int productId;
    private String productName;

    /** 등록 날짜 */
    private String createdDate;

    /** 등록자 */
    private int createdBy;
    private String createdByNm;

    /** 수정일 */
    private String modifiedDate;

    /** 수정자 */
    private int modifiedBy;
    private String modifiedByNm;

    /** 삭제 여부 */
    private String isDeleted;

    /** 사용 여부 */
    private String useYn;

    /** 기본 여부 */
    private String isDefault;

	private ProductsVO product;

	private int systemMapCount;
    private int usedSystemMapCount;

	public void convertUTCtoUserTime() {
		this.createdDate = DateUtil.convertUTCtoUserTime(this.createdDate);
		this.modifiedDate = DateUtil.convertUTCtoUserTime(this.modifiedDate);
	}

	public void convertUserTimeToUTC() {
		this.createdDate = DateUtil.convertUserTimeToUTC(this.createdDate);
		this.modifiedDate = DateUtil.convertUserTimeToUTC(this.modifiedDate);
	}
}

