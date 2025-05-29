package egovframework.com.primx.fun.sys.service;

import java.util.Date;

import egovframework.com.primx.fun.cmm.service.DataTableBaseVO;

/**
 * @Class Name : SystemMasterVO.java
 * @Description : Agent가 설치된 시스템 정보 테이블 (T_SYSTEM_MASTER)
 * <pre>
 * 주요 컬럼:
 * - C_SEQ_NO : ID
 * - C_AGENT_ID : Agent ID
 * - C_WIN_VERSION_STRING : Windows 버전
 * - C_UUID : Windows UUID
 * - C_MAJOR_VERSION : OS Major 버전
 * - C_MINOR_VERSION : OS Minor 버전
 * - C_BUILD_NUMBER : OS Build 번호
 * - C_SERVICEPACK_MAJOR : OS 서비스팩 Major 버전
 * - C_SERVICEPACK_MINOR : OS 서비스팩 Minor 버전
 * - C_PROCESSOR_ARCHITECTURE : OS 프로세서 아키텍처
 * - C_RELEASE_ID : OS Release ID
 * - C_COMPUTER_NAME : 컴퓨터 이름
 * - C_WORKGROUP_NAME : 작업그룹 이름
 * - C_ENCRYPTION_KEY : 암호 키
 * - C_AGENT_VERSION : Agent 버전
 * - C_AGENT_STATE : Agent 상태
 * - C_LAST_KA_DATE : 마지막 Keep Alive 시간
 * - C_COMMON_SYSTEM_YN : 공용 PC 여부
 * - C_USE_YN : 정보 사용 여부
 * - C_UNINSTALL_YN : Agent 삭제 여부
 * - C_MODIFIED_BY : 수정자 User ID
 * - C_MODIFIED_IP : 수정자 IP
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
 *   2024.12.04        김영우     최초 생성
 *</pre>
 *
 *  @author 김영우
 *  @since 2024.12.04
 *  @version 1.0
 *  @see
 *  
 */
public class SystemMasterVO extends DataTableBaseVO {
    private static final long serialVersionUID = 1L;

    /** ID */
    private long seqNo;

    /** Agent ID */
    private String agentId;

    /** Windows 버전 */
    private String winVersionString;

    /** Windows UUID */
    private String uuid;

    /** OS Major 버전 */
    private int majorVersion;

    /** OS Minor 버전 */
    private int minorVersion;

    /** OS Build 번호 */
    private String buildNumber;

    /** OS 서비스팩 Major 버전 */
    private int servicepackMajor;

    /** OS 서비스팩 Minor 버전 */
    private int servicepackMinor;

    /** OS 프로세서 아키텍처 */
    private int processorArchitecture;

    /** OS Release ID */
    private String releaseId;

    /** 컴퓨터 이름 */
    private String computerName;

    /** 작업그룹 이름 */
    private String workgroupName;

    /** 암호 키 */
    private String encryptionKey;

    /** Agent 버전 */
    private String agentVersion;

    /** Agent 상태 */
    private int agentState;

    /** 마지막 Keep Alive 시간 */
    private Date lastKaDate;

    /** 공용 PC 여부 */
    private String commonSystemYn;

    /** 정보 사용 여부 */
    private String useYn;

    /** Agent 삭제 여부 */
    private String uninstallYn;

    /** 수정자 User ID */
    private String modifiedBy;

    /** 수정자 IP */
    private String modifiedIp;

    /** 생성 일자 */
    private Date insertDate;

    /** 수정 일자 */
    private Date updateDate;

	public long getSeqNo() {
		return seqNo;
	}

	public void setSeqNo(long seqNo) {
		this.seqNo = seqNo;
	}

	public String getAgentId() {
		return agentId;
	}

	public void setAgentId(String agentId) {
		this.agentId = agentId;
	}

	public String getWinVersionString() {
		return winVersionString;
	}

	public void setWinVersionString(String winVersionString) {
		this.winVersionString = winVersionString;
	}

	public String getUuid() {
		return uuid;
	}

	public void setUuid(String uuid) {
		this.uuid = uuid;
	}

	public int getMajorVersion() {
		return majorVersion;
	}

	public void setMajorVersion(int majorVersion) {
		this.majorVersion = majorVersion;
	}

	public int getMinorVersion() {
		return minorVersion;
	}

	public void setMinorVersion(int minorVersion) {
		this.minorVersion = minorVersion;
	}

	public String getBuildNumber() {
		return buildNumber;
	}

	public void setBuildNumber(String buildNumber) {
		this.buildNumber = buildNumber;
	}

	public int getServicepackMajor() {
		return servicepackMajor;
	}

	public void setServicepackMajor(int servicepackMajor) {
		this.servicepackMajor = servicepackMajor;
	}

	public int getServicepackMinor() {
		return servicepackMinor;
	}

	public void setServicepackMinor(int servicepackMinor) {
		this.servicepackMinor = servicepackMinor;
	}

	public int getProcessorArchitecture() {
		return processorArchitecture;
	}

	public void setProcessorArchitecture(int processorArchitecture) {
		this.processorArchitecture = processorArchitecture;
	}

	public String getReleaseId() {
		return releaseId;
	}

	public void setReleaseId(String releaseId) {
		this.releaseId = releaseId;
	}

	public String getComputerName() {
		return computerName;
	}

	public void setComputerName(String computerName) {
		this.computerName = computerName;
	}

	public String getWorkgroupName() {
		return workgroupName;
	}

	public void setWorkgroupName(String workgroupName) {
		this.workgroupName = workgroupName;
	}

	public String getEncryptionKey() {
		return encryptionKey;
	}

	public void setEncryptionKey(String encryptionKey) {
		this.encryptionKey = encryptionKey;
	}

	public String getAgentVersion() {
		return agentVersion;
	}

	public void setAgentVersion(String agentVersion) {
		this.agentVersion = agentVersion;
	}

	public int getAgentState() {
		return agentState;
	}

	public void setAgentState(int agentState) {
		this.agentState = agentState;
	}

	public Date getLastKaDate() {
		return lastKaDate;
	}

	public void setLastKaDate(Date lastKaDate) {
		this.lastKaDate = lastKaDate;
	}

	public String getCommonSystemYn() {
		return commonSystemYn;
	}

	public void setCommonSystemYn(String commonSystemYn) {
		this.commonSystemYn = commonSystemYn;
	}

	public String getUseYn() {
		return useYn;
	}

	public void setUseYn(String useYn) {
		this.useYn = useYn;
	}

	public String getUninstallYn() {
		return uninstallYn;
	}

	public void setUninstallYn(String uninstallYn) {
		this.uninstallYn = uninstallYn;
	}

	public String getModifiedBy() {
		return modifiedBy;
	}

	public void setModifiedBy(String modifiedBy) {
		this.modifiedBy = modifiedBy;
	}

	public String getModifiedIp() {
		return modifiedIp;
	}

	public void setModifiedIp(String modifiedIp) {
		this.modifiedIp = modifiedIp;
	}

	public Date getInsertDate() {
		return insertDate;
	}

	public void setInsertDate(Date insertDate) {
		this.insertDate = insertDate;
	}

	public Date getUpdateDate() {
		return updateDate;
	}

	public void setUpdateDate(Date updateDate) {
		this.updateDate = updateDate;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

	@Override
	public String toString() {
		return "SystemMasterVO [seqNo=" + seqNo + ", agentId=" + agentId + ", winVersionString=" + winVersionString
				+ ", uuid=" + uuid + ", majorVersion=" + majorVersion + ", minorVersion=" + minorVersion
				+ ", buildNumber=" + buildNumber + ", servicepackMajor=" + servicepackMajor + ", servicepackMinor="
				+ servicepackMinor + ", processorArchitecture=" + processorArchitecture + ", releaseId=" + releaseId
				+ ", computerName=" + computerName + ", workgroupName=" + workgroupName + ", encryptionKey="
				+ encryptionKey + ", agentVersion=" + agentVersion + ", agentState=" + agentState + ", lastKaDate="
				+ lastKaDate + ", commonSystemYn=" + commonSystemYn + ", useYn=" + useYn + ", uninstallYn="
				+ uninstallYn + ", modifiedBy=" + modifiedBy + ", modifiedIp=" + modifiedIp + ", insertDate="
				+ insertDate + ", updateDate=" + updateDate + "]";
	}
    
}
