<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.systemInfo">PRIMX | 시스템 정보</title>
<form class="mySystemInfo" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.systemInfo">시스템 정보</h2>
		</div>
	</div>
	<div class="contDiv">
		<h3 class="contTitle" data-i18n="systemInfo.generalInfo">일반 정보</h3>
		<div class="tableWrap leftTable">
			<table>
				<colgroup>
					<col width="20%" />
					<col width="80%" />
				</colgroup>
				<tr>
					<th scope="row" data-i18n="systemInfo.workGroup">작업 그룹</th>
					<td id="viewWorkgroupName"></td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemInfo.usedDomain">사용 도메인</th>
					<td></td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemInfo.agentVersion">Agent 버전</th>
					<td id="viewAgentVersion"></td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemInfo.lastAgentPolicyName">최종 Agent정책명</th>
					<td id="viewAgentPolicyName"></td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemInfo.user">사용자</th>
					<td id="viewUserNm"></td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemInfo.lastLoginDate">최종 접속 일시</th>
					<td id="viewLastKaDate"></td>
				</tr>
			</table>
		</div>
	</div>
	<div class="contDiv">
		<h3 class="contTitle" data-i18n="systemInfo.osInfo">OS 정보</h3>
		<div class="tableWrap leftTable">
			<table>
				<colgroup>
					<col width="20%" />
					<col width="80%" />
				</colgroup>
				<tr>
					<th scope="row" data-i18n="systemInfo.windowsVersion">Windows버전</th>
					<td id="viewWinVersionString"></td>
				</tr>
				<tr>
					<th scope="row">Windows UUID</th>
					<td id="viewWinUuid"></td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemInfo.osMajorVersion">OS 주 버전</th>
					<td id="viewOsMajorVersion"></td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemInfo.osBuildNumber">OS 빌드번호</th>
					<td id="viewOsBuildNumber"></td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemInfo.osServicePackMajorVersion">OS 서비스팩 주 버전</th>
					<td id="viewOsServicepackMajor"></td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemInfo.osServicePackMinorVersion">OS 서비스팩 부 버전</th>
					<td id="viewOsServicepackMinor"></td>
				</tr>
			</table>
		</div>
	</div>
	<div class="contDiv">
		<h3 class="contTitle" data-i18n="systemInfo.diskInfo">디스크 정보</h3>
		<div class="tableWrap leftTable doubleTable">
			<table id="diskInfoTable">
				<colgroup>
					<col width="15%" />
					<col width="20%" />
					<col width="65%" />
				</colgroup>
				<tbody>
					<tr>
						<th scope="row" rowspan="5" class="firstTh">Disk <br> <span></span></th>
						<th scope="row" data-i18n="systemInfo.diskType">디스크 종류</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row" data-i18n="systemInfo.volumeName">볼륨명</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row" data-i18n="systemInfo.fileSystem">파일시스템</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row" data-i18n="systemInfo.totalSize">전체사이즈</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row" data-i18n="systemInfo.availableSize">사용가능 사이즈</th>
						<td></td>
					</tr>
				</tbody>
			</table>
		</div>
	</div>
	<div class="contDiv">
		<h3 class="contTitle" data-i18n="systemInfo.nicInfo">NIC 정보</h3>
		<div class="tableWrap leftTable doubleTable">
			<table id="nicInfoTable">
				<colgroup>
					<col width="15%" />
					<col width="20%" />
					<col width="65%" />
				</colgroup>
				<tbody>
					<tr>
						<th scope="row" rowspan="6" class="firstTh">MAC</th>
						<th scope="row">Mac</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row" data-i18n="systemInfo.ipAddress">IP주소</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row">Gateway</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row">DNS</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row" data-i18n="systemInfo.dhcpServerAddress">DHCP 서버주소</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row">Device Name</th>
						<td></td>
					</tr>
				</tbody>
			</table>
		</div>
	</div>
	<div class="contDiv">
		<h3 class="contTitle" data-i18n="systemInfo.productInfo">제품 정보</h3>
		<div class="tableWrap leftTable doubleTable">
			<table id="setupInfoTable">
				<colgroup>
					<col width="15%" />
					<col width="20%" />
					<col width="65%" />
				</colgroup>
				<tbody>
					<tr>
						<th scope="row" rowspan="3" class="firstTh">ZONECENTRAL</th>
						<th scope="row" data-i18n="systemInfo.productVersion">제품 버전</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row" data-i18n="systemInfo.lastAppliedPolicyName">최종 적용 정책명</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row" data-i18n="systemInfo.lastPolicyAppliedDate">최종 정책 적용 일자</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row" rowspan="3" class="firstTh">Agent</th>
						<th scope="row" data-i18n="systemInfo.productVersion">제품 버전</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row" data-i18n="systemInfo.lastAppliedPolicyName">최종 적용 정책명</th>
						<td></td>
					</tr>
					<tr>
						<th scope="row" data-i18n="systemInfo.lastPolicyAppliedDate">최종 정책 적용 일자</th>
						<td></td>
					</tr>
				</tbody>
			</table>
		</div>
	</div>
</form>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/usr/mySystemInfo.js' />"></script>