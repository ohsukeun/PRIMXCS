/* 신청 및 결재 > 신청 */
var table = null;
var deptUserTable = null;

function pageInit(){
	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();

	loadMySystemInfo();
}

function loadMySystemInfo() {
	$.ajax({
		url: '/primx/fun/usr/mySystemInfo.do',
		type: 'POST',
		success: function(response) {
			console.log(response);
			
			// 시스템 정보 세팅
			var sys = response.system;
			$('#viewInsertDate').text(sys.insertDate);
			$('#viewCommonSystemType').text((sys.commonSystemYn == 'Y' ? '공용' : '사용자'));
			$('#viewAgentId').text(sys.agentId);

			$('#viewComputerName').text(sys.computerName);

			// 일반정보 > 작업그룹
			$('#viewWorkgroupName').text(sys.workgroupName);
			$('#viewDomainName').text(sys.computerName);
			$('#viewAgentVersion').text(sys.agentVersion);
			$('#viewAgentPolicyName').text(sys.agentPolicyName);
			$('#viewUserNm').text(sys.userNm);
			$('#viewLastKaDate').text(sys.lastKaDate);

			// OS 정보
			$('#viewWinVersionString').text(sys.winVersionString);
			$('#viewWinUuid').text(sys.winUuid);
			$('#viewOsMajorVersion').text(sys.osMajorVersion);
			$('#viewOsBuildNumber').text(sys.osBuildNumber);
			$('#viewOsServicepackMajor').text(sys.osServicepackMajor);
			$('#viewOsServicepackMinor').text(sys.osServicepackMinor);

			// DISK 정보
			var diskList = response.diskInfo;
			$('#diskInfoTable tbody').empty();
			for(var i = 0; i < diskList.length; i++) {
				var disk = diskList[i];

				var diskType;
				if (disk.diskType == 2) {
					diskType = '이동식 디스크';
				} else if (disk.diskType == 3) {
					diskType = '고정 디스크';
				} else if (disk.diskType == 5) {
					diskType = 'CD-ROM';
				} else if (disk.diskType == 6) {
					diskType = 'RAM 디스크';
				} else {
					diskType = '알 수 없음';
				}

				$('#diskInfoTable tbody').append(`
					<tr>
						<th scope="row" rowspan="5" class="firstTh">디스크명 <br> <span>${disk.diskName }</span></th>
						<th scope="row" data-i18n="systemStatus.systemInformationPopup.diskType">디스크 종류</th>
						<td>${diskType }</td>
					</tr>
					<tr>
						<th scope="row" data-i18n="systemStatus.systemInformationPopup.volumeName">볼륨명</th>
						<td>${disk.volumnName }</td>
					</tr>
					<tr>
						<th scope="row">파일시스템</th>
						<td>${disk.fileSystemName }</td>
					</tr>
					<tr>
						<th scope="row">전체사이즈</th>
						<td>${convertByteUnitWithComma(disk.totalSize, 'MB') }</td>
					</tr>
					<tr>
						<th scope="row">사용가능 사이즈</th>
						<td>${convertByteUnitWithComma(disk.freeSize, 'MB') }</td>
					</tr>
				`);
			}


			// NIC 정보
			var nicList = response.nicInfo;
			$('#nicInfoTable tbody').empty();
			for(var i = 0; i < nicList.length; i++) {
				var nic = nicList[i];
				$('#nicInfoTable tbody').append(`
					<tr>
						<th scope="row" rowspan="6" class="firstTh">MAC</th>
						<th scope="row">Mac</th>
						<td>${nic.mac }</td>
					</tr>
					<tr>
						<th scope="row">IP주소</th>
						<td>${nic.ip }</td>
					</tr>
					<tr>
						<th scope="row">Gateway</th>
						<td>${nic.gateway }</td>
					</tr>
					<tr>
						<th scope="row">DNS</th>
						<td>${nic.dns }</td>
					</tr>
					<tr>
						<th scope="row">DHCP 서버주소</th>
						<td>${nic.dhcpServer }</td>
					</tr>
					<tr>
						<th scope="row">Device Name</th>
						<td>${nic.name }</td>
					</tr>
				`);
			}

			// 제품 정보
			var primxSetupInfoList = response.primxSetupInfo;
			$('#setupInfoTable tbody').empty();
			for(var i = 0; i < primxSetupInfoList.length; i++) {
				var primxSetupInfo = primxSetupInfoList[i];
				$('#setupInfoTable tbody').append(`
					<tr>
						<th scope="row" rowspan="3" class="firstTh">${primxSetupInfo.productName }</th>
						<th scope="row">제품 버전</th>
						<td>${primxSetupInfo.productVersion }</td>
					</tr>
					<tr>
						<th scope="row">최종 적용 정책명</th>
						<td>${primxSetupInfo.policyName }</td>
					</tr>
					<tr>
						<th scope="row">최종 정책 적용 일자</th>
						<td>${primxSetupInfo.updateDate ? primxSetupInfo.updateDate : ''}</td>
					</tr>
				`);
			}
			

		}
	});

}

