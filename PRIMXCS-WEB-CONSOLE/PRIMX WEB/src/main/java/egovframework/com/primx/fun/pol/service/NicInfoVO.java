package egovframework.com.primx.fun.pol.service;

import java.io.Serializable;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * @Class Name : NicInfoVO.java
 * @Description : Agent NIC 정보 테이블 (T_NIC_INFO)
 * <pre>
 * 주요 컬럼:
 * - C_SEQ_NO : ID
 * - C_AGENT_ID : Agent ID
 * - C_IP : IP 주소
 * - C_MAC : MAC 주소
 * - C_NAME : 어뎁터 이름
 * - C_GATEWAY : 게이트웨이
 * - C_DNS : DNS 서버 주소
 * - C_DHCP_ENABLE : DHCP 사용 여부
 * - C_DHCP_SERVER : DHCP 서버 주소
 * - C_INSERT_DATE : 생성 일자
 * - C_UPDATE_DATE : 수정 일자
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
public class NicInfoVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private long seqNo;

    /** Agent ID */
    private String agentId;

    /** IP 주소 */
    private String ip;

    /** MAC 주소 */
    private String mac;

    /** 어뎁터 이름 */
    private String name;

    /** 게이트웨이 */
    private String gateway;

    /** DNS 서버 주소 */
    private String dns;

    /** DHCP 사용 여부 */
    private int dhcpEnable;

    /** DHCP 서버 주소 */
    private String dhcpServer;

    /** 생성 일자 */
    private String insertDate;

    /** 수정 일자 */
    private String updateDate;

	public void convertUTCtoUserTime() {
		this.insertDate = DateUtil.convertUTCtoUserTime(this.insertDate);
		this.updateDate = DateUtil.convertUTCtoUserTime(this.updateDate);
	}
    
}
