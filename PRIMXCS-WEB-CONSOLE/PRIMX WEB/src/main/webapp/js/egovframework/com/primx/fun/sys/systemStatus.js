/* 현황 > 시스템 현황 */
var table = null;

var productPolicyTable = null;
var agentPolicyTable = null;

var productHistoryDataTable = null;
var moveFileHistoryTable = null;
var originalFileInfoTable = null;
var moveFileUsageHistoryTable = null;

var companyId = null;

/* csv 다운로드 제목포멧 */
var firstDepthTit = $(".left nav .firstDepthMenu > li.select > p").text();
var secondDepthTit = $(".left nav .firstDepthMenu > li.select .secondDepthMenu > li.select a").text();
var today = new Date();
var formattedDate = today.getFullYear() + '-' +
    ('0' + (today.getMonth() + 1)).slice(-2) + '-' +
    ('0' + today.getDate()).slice(-2);
    

function pageInit(){
	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();
	initMenuBtnReset();

	const companyData = loadRootCompanyInfo();
	companyId = companyData.companyId;

	/* dataTables 초기화 */
	table = initDataTables('#grid01');
	productHistoryDataTable = initDataTables('#productHistoryDataTable');
	/* datepicker */
	datePicker();

	/* 시스템현황tab 활성화 */
	$(".systabWrap .tabTitle li").on("click", function(){
		var sysTabNum = $(".systabWrap .tabTitle li").index($(this));//click하는 인덱스
		$(".systabWrap .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
		$(".systabWrap .tabCont > li:eq(" + (sysTabNum) + ")").addClass("on");
		$(".systabWrap .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
		$(this).addClass("on");
	});
	
	/* 이동파일 정보 tab 활성화 */
	$(".moveFileTabWrap .tabTitle li").on("click", function(){
		var sysTabNum = $(".moveFileTabWrap .tabTitle li").index($(this));//click하는 인덱스
		$(".moveFileTabWrap .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
		$(".moveFileTabWrap .tabCont > li:eq(" + (sysTabNum) + ")").addClass("on");
		$(".moveFileTabWrap .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
		$(this).addClass("on");
	});

	/** 목록 조회 */
	fnSearchList();

	/* 왼쪽메뉴 셀렉트 되도록 */
	var firstDepth = $(".firstDepthMenu .current");
	$(".left li").removeClass("select");
	$(firstDepth).closest("li").addClass("select open");
	$(".systemStatusLi").addClass("select");
}

/**
 * 목록 조회
 */
function fnSearchList() {
	// 기존 테이블이 있다면 제거
	if (table) {
		table.destroy();
		table = null;
	}	

	// 기존 CSV 버튼 이벤트 바인딩 제거
	$('#sysStatusExport').off('click');

	table = initDataTables('#grid01' ,{
		ajax: {
			url: '/primx/fun/sys/systemStatus/list.do',
			type: 'POST',
			data: function(data) {
				data.startDt = $('#startDt').val();
				data.endDt = $('#endDt').val();
				data.lastKaDateType = $('#lastKaDateType').val();
				data.commonSystemYn = $('#commonSystemYn').val();
				data.productId = $('#productName').val() == '' ? -1 : $('#productName').val();
				data.productName = ($('#productName').val() != '' ? $('#productName option:selected').text() : '');
				data.searchType = $('#searchType').val();
				data.searchWord = $('#searchWord').val();
			},
			dataSrc: function (json) {
				// 일련번호 생성
				var data = json.data;
				for (var i = 0; i < data.length; i++) {
					var idx = json.recordsTotal - ((json.page-1) * json.pageSize) - i;
					data[i].rowId = idx;
				}
				return data;
			}
		},
		processing: true,
    	serverSide: true,
		columns: [
	        { data: null },
	        { data: 'rowId' },
	        { data: 'agentId' },
	        { data: 'computerName' },
	        {
				data: null,
				render: function (data, type, row, meta) {
					return getPolicyZone(row, 'ZC');
				}
			},
	        /* {
				data: null,
				render: function (data, type, row, meta) {
					return getPolicyZone(row, 'OZ');
				}
			}, */
	        {
				data: 'commonSystemYn',
				render: function (data, type, row, meta) {
					if (data == 'Y') {
						return '<span class="common" title="공용">공용</span>';
					} else {
						return '<span class="user" title="사용자">사용자</span>';
					}
				},
				createdCell: function (td, cellData, rowData, row, col) {
					$(td).addClass('system-type');
				}
	        },
			{
				data: 'companyId',
				render: function (data, type, row, meta) {
					// 회사코드가 해당 코드와 다르면 외부자, 같으면 내부자
					if (companyId != row.companyId) {
						return '외부';
					} else {
						return '내부';
					}
				}
			},
	        { data: 'userNm' },
	        { data: 'deptName' },
	        { data: 'winVersionString' },
	        { data: 'agentVersion' }
	        // { data: 'agentPolicyName' },
	        // {
			// 	data: 'insertDate',
			// 	render: function(data, type, row, meta) {
			// 		return moment(data).format('YYYY-MM-DD');
			// 	}
			// },
	        // {
			// 	data: 'lastKaDate',
			// 	render: function(data, type, row, meta) {
			// 		return moment(data).format('YYYY-MM-DD HH:mm:ss');
			// 	}
			// }
	    ],
		columnDefs: [
			{
				render: DataTable.render.select(),
				targets: 0
			}
		],
		buttons: [
			{
				extend: 'csv',
				title: `${firstDepthTit}_${secondDepthTit}_${formattedDate}`,
				exportOptions: {
					columns: ':not(:first-child)'  // 첫 번째 컬럼(체크박스)을 제외한 모든 컬럼 내보내기
				},
				bom: true, // UTF-8 BOM 추가
				customize: function(csv) {
					return '\ufeff' + csv; // UTF-8 BOM 추가하여 한글 깨짐 방지
				}
			}
		],
		select: {
			style: 'multi',
			selector: 'td:first-child'
		},
		"drawCallback": function() {
      		eventRegist();
    	}
	});
	
	// CSV 내보내기 버튼과 연결
    $('#sysStatusExport').on('click', function() {
        table.button('.buttons-csv').trigger();
    });
}

/**
 * 목록 생성후 이벤트 등록
 */
function eventRegist() {
	// td 클릭시 상세보기 팝업
	$('#grid01 tbody').off('click');
	$('#grid01 tbody').on('click', 'td:not(:first-child)', function () {
		var table = $('#grid01').DataTable();
		var row = table.row(this).data();           // returns undefined
		viewData(row);
    });

	// checkbox 체크시 이벤트용
	$('#grid01 input[type="checkbox"]').off();
    $('#grid01 input[type="checkbox"]').on('change', function() {
	    console.log('checkbox change');
	    initMenuBtnReset();
	    var data = table.rows({ selected: true }).data();

	    if (data.length > 1) {
			$('#policyChangeBtn').removeClass('disabled');
			$('.policyChangeBtn').removeClass('disable');
			$('#policyChangeBtn').on('click', function(){
				changePolicy(data);
			});
			$('#agentChangeBtn').removeClass('disabled');
			$('.agentChangeBtn').removeClass('disable');
			$('#agentChangeBtn').on('click', function(){
				$('.agentChangePopup').removeClass('hide');
			});
		} else if (data.length == 1) {
			$('#sysTypeBtn').removeClass('disabled');
			$('.sysTypeBtn').removeClass('disable');
			$('#sysTypeBtn').on('click', function(){
				updateSystemType(data[0]);
			});
			$('#sysDelBtn').removeClass('disabled');
			$('.sysDelBtn').removeClass('disable');
			$('#sysDelBtn').on('click', function(){

				$('.sysDelPopup').removeClass('hide');
			});
			$('#policyChangeBtn').removeClass('disabled');
			$('.policyChangeBtn').removeClass('disable');
			$('#policyChangeBtn').on('click', function(){
				changePolicy(data);
			});
			$('#agentChangeBtn').removeClass('disabled');
			$('.agentChangeBtn').removeClass('disable');
			$('#agentChangeBtn').on('click', function(){
				changeAgentPolicy(data);
			});
			$('#productHistoryBtn').removeClass('disabled');
			$('.productHistoryBtn').removeClass('disable');
			$('#productHistoryBtn').on('click', function(){
				$('.productHistoryPopup').removeClass('hide');
				searchSystemStatus('productHistorySearchPop', data[0].userNm);
			});
			$('#moveFileHistoryBtn').removeClass('disabled');
			$('.moveFileHistoryBtn').removeClass('disable');
			$('#moveFileHistoryBtn').on('click', function(){
				$('.moveFileHistoryPopup').removeClass('hide');
				$(".moveFileHistorySearchPop .search_wrap select[name='condition']").val('USER_NAME');
				$(".moveFileHistorySearchPop .search_wrap input[name='searchKeyword']").val(data[0].userNm);
				
				searchSystemStatus('moveFileHistorySearchPop', data[0].userNm);
			});
		}
	});
}

function initMenuBtnReset() {
	$('#sysTypeBtn').addClass('disabled');
	$('.sysTypeBtn').addClass('disable');
	$('#sysDelBtn').addClass('disabled');
	$('.sysDelBtn').addClass('disable');
	$('#policyChangeBtn').addClass('disabled');
	$('.policyChangeBtn').addClass('disable');
	$('#agentChangeBtn').addClass('disabled');
	$('.agentChangeBtn').addClass('disable');
	$('#productHistoryBtn').addClass('disabled');
	$('.productHistoryBtn').addClass('disable');
	$('#moveFileHistoryBtn').addClass('disabled');
	$('.moveFileHistoryBtn').addClass('disable');

	// 클릭 이벤트 제거
	$('#sysTypeBtn').off('click');
	$('#sysDelBtn').off('click');
	$('#policyChangeBtn').off('click');
	$('#agentChangeBtn').off('click');
	$('#productHistoryBtn').off('click');
	$('#moveFileHistoryBtn').off('click');
}

/**
 * policy ZC, OZ 체크
 */
function getPolicyZone(row, type) {
	var policys = row.policys;
	var rtnStr = '';
	if (policys) {
		if (policys.includes(',')) {
			var policyArr = policys.split(',');
			for (var i = 0; i < policyArr.length; i++) {
				var productId = policyArr[i].split('||')[2];
				if (type == 'ZC' && productId == 1) {
					rtnStr = '<span class="checkIco" title="'+ policyArr[i].split('||')[1] +'">'+ policyArr[i].split('||')[1] +'</span>';
					break;
				} else if (type == 'OZ' && productId == 2) {
					rtnStr = '<span class="checkIco" title="'+ policyArr[i].split('||')[1] +'">'+ policyArr[i].split('||')[1] +'</span>';
					break;
				}
			}
		} else {
			var productId = policys.split('||')[2];
			if (type == 'ZC' && productId == 1) {
				rtnStr = '<span class="checkIco" title="'+ policys.split('||')[1] +'">'+ policys.split('||')[1] +'</span>';
			} else if (type == 'OZ' && productId == 2) {
				rtnStr = '<span class="checkIco" title="'+ policys.split('||')[1] +'">'+ policys.split('||')[1] +'</span>';
			}
		}
	}
	return rtnStr;
}

/**
 * 상세보기
 */
function viewData(rowData) {
	if (rowData == null) {
		return;
	}

    $.ajax({
        url: '/primx/fun/sys/systemStatus/info.do?systemId='+ rowData.systemId,
        method: 'GET',
        success: function (data) {

			// 시스템 정보 세팅
			var sys = data.system;
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
			var diskList = data.diskInfo;
			$('#diskInfoTable tbody').empty();
			for(var i = 0; i < diskList.length; i++) {
				var disk = diskList[i];
				// (0: 알 수 없음, 2: 이동식 디스크, 3: 고정 디스크, 5: CD-ROM, 6:RAM 디스크)
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

				var tables = `<tr>
					<th scope="row" rowspan="5" class="firstTh">Disk <br> <span>${disk.diskName }</span></th>
					<th scope="row" data-i18n="systemStatus.systemInformationPopup.diskType">디스크 종류</th>
					<td>${diskType }</td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemStatus.systemInformationPopup.volumeName">볼륨명</th>
					<td>${disk.volumnName }</td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemStatus.systemInformationPopup.fileSystem">파일시스템</th>
					<td>${disk.fileSystemName }</td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemStatus.systemInformationPopup.totalSize">전체사이즈</th>
					<td>${ convertByteUnitWithComma(disk.totalSize, 'MB') }</td>
				</tr>
				<tr>
					<th scope="row" data-i18n="systemStatus.systemInformationPopup.availableSize">사용가능 사이즈</th>
					<td>${ convertByteUnitWithComma(disk.freeSize, 'MB') }</td>
				</tr>`;
				
				$('#diskInfoTable tbody').append(tables);
				
				
			}

			// NIC 정보
			var nicList = data.nicInfo;
			$('#nicInfoTable tbody').empty();
			for(var i = 0; i < nicList.length; i++) {
				var nic = nicList[i];
				var tables = `<tr>
							<th scope="row" rowspan="6" class="firstTh">MAC</th>
							<th scope="row">Mac</th>
							<td>${nic.mac }</td>
						</tr>
						<tr>
							<th scope="row" data-i18n="systemStatus.systemInformationPopup.ipAddress">IP주소</th>
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
							<th scope="row" data-i18n="systemStatus.systemInformationPopup.dhcpServerAddress">DHCP 서버주소</th>
							<td>${nic.dhcpServer }</td>
						</tr>
						<tr>
							<th scope="row">Device Name</th>
							<td>${nic.name }</td>
						</tr>`;
				$('#nicInfoTable tbody').append(tables);
			}
			
			// 제품 정보
			var setupList = data.primxSetupInfo;
			$('#setupInfoTable tbody').empty();
			for(var i = 0; i < setupList.length; i++) {
				var setup = setupList[i];
				var tables = `<tr>
							<th scope="row" rowspan="3" class="firstTh">${setup.productName }</th>
							<th scope="row" data-i18n="systemStatus.systemInformationPopup.productVersion">제품 버전</th>
							<td>${setup.productVersion }</td>
						</tr>
						<tr>
							<th scope="row" data-i18n="systemStatus.systemInformationPopup.lastAppliedPolicyName">최종 적용 정책명</th>
							<td>${setup.policyName }</td>
						</tr>
						<tr>
							<th scope="row" data-i18n="systemStatus.systemInformationPopup.lastPolicyAppliedDate">최종 정책 적용 일자</th>
							<td>${setup.updateDate ? setup.updateDate : ''}</td>
						</tr>`;
				$('#setupInfoTable tbody').append(tables);
			}
			// 추가된 부분: 동적으로 생성된 HTML 요소에 다국어 적용
        	applyTranslations();
			
			// systabWrap 탭 가장 첫번째 선택
			$(".systabWrap .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
			$(".systabWrap .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
			$('.systabWrap .tabTitle li:first-child').addClass("on"); //첫번째 li에 테두리를 추가해라
			$('.systabWrap .tabCont > li:first-child').addClass("on"); //첫번째 탭을 선택해라

			// 정보 확인 팝업
			$(".systemInformationPopup").removeClass("hide");
        },
        error: function (err) {
			alert('ajax call error');
        }
    });
}


function escapeHtml(text) {
    if (!text) return '';
    return text
        .replace(/&/g, "&amp;")
        .replace(/</g, "&lt;")
        .replace(/>/g, "&gt;")
        .replace(/"/g, "&quot;")
        .replace(/'/g, "&#039;");
}

function initProductHistoryDataTable() {
	// 기존 테이블이 있다면 제거
	if (productHistoryDataTable) {
		productHistoryDataTable.destroy();
		productHistoryDataTable = null;
	}

	// 기존 CSV 버튼 이벤트 바인딩 제거
	$('#productHistoryPopupCsvExport').off('click');

	productHistoryDataTable = initDataTables('#productHistoryDataTable', {
		processing: true,
		serverSide: true,
		destroy: true,  /* 기존 인스턴스 제거 옵션 추가 */
		ajax: {
			url: '/primx/fun/log/productLog/list.do',
			type: 'POST',
			data: function(d) {
				console.log("DataTables sending params:", d);
				/* 검색 조건 추가 */
				const searchParams = {
					searchDateType: $(".productHistorySearchPop select[name='dateSelectPopProduct']").val(),
					searchStartDate: $(".productHistorySearchPop input[name='startDt']").val(),
					searchEndDate: $(".productHistorySearchPop input[name='endDt']").val(),
					searchLevel: $(".productHistorySearchPop select[name='searchLevel']").val(),
					searchProduct: $(".productHistorySearchPop select[name='searchProduct']").val(),
					searchType: $(".productHistorySearchPop select[name='searchType']").val(),
					searchCondition: $(".productHistorySearchPop select[name='condition']").val(),
					searchKeyword: $(".productHistorySearchPop input[name='searchKeyword']").val()
				};
				console.log("Search params:", searchParams);
				return {...d, ...searchParams};
			}
		},
		columns: [
			{ data: null, defaultContent: '' },
			{ data: 'logDate' },
			{ data: 'logTime' },
			{ data: 'productName' },
			{ data: 'logLevel' },
			{ data: 'logName' },
			{ 
				data: 'logText',
				render: function(data, type, row) {
					if (type === 'display') {
						return escapeHtml(data);
					}
					return data;
				}
			},
			{ data: 'computerName' },
			{ data: 'deptName' },
			{ data: 'userName' }
		],
		columnDefs: [
			{
				render: DataTable.render.select(),
				targets: 0
			}
		],
		buttons: [
			{
				extend: 'csv',
				title: `${firstDepthTit}_${secondDepthTit}_${formattedDate}`,
				exportOptions: {
					columns: ':not(:first-child)'  // 첫 번째 컬럼(체크박스)을 제외한 모든 컬럼 내보내기
				},
				bom: true, // UTF-8 BOM 추가
				customize: function(csv) {
					return '\ufeff' + csv; // UTF-8 BOM 추가하여 한글 깨짐 방지
				}
			}
		],
		select: {
			style: 'multi',
			selector: 'td:first-child'
		}
	});

	$('#productHistoryDataTable tbody').off('click');
	$('#productHistoryDataTable tbody').on('click', 'td:not(:first-child)', function() {
		const data = productHistoryDataTable.row(this).data(); // 클릭한 행의 데이터 가져오기
		if (data) {
			loadProductHistoryDetails(data); // 사용자 정보를 로드하는 함수 호출
		}
	});
	
	// CSV 내보내기 버튼과 연결
    $('#productHistoryPopupCsvExport').on('click', function() {
        productHistoryDataTable.button('.buttons-csv').trigger();
    });

}

function loadProductHistoryDetails(data) {
	// 팝업의 필드에 데이터 채우기
	$(".productHistoryViewPopup .pop_cont .item em").each(function() {
		const fieldName = $(this).text(); // 필드 이름 가져오기
		switch (fieldName) {
			case "일시":
				$(this).next().text(data.logDate + ' ' + data.logTime); // 일시
				break;
			case "레벨":
				$(this).next().text(data.logLevel); // 레벨
				break;
			case "제품":
				$(this).next().text(data.productName); // 제품
				break;
			case "종류":
				$(this).next().text(data.logName); // 종류
				break;
			case "내용":
				$(this).next().text(data.logText); // 내용
				break;
			case "시스템명":
				$(this).next().text(data.computerName); // 시스템명
				break;
			case "사용자명":
				$(this).next().text(data.userName); // 사용자명
				break;
			case "작업자명":
				$(this).next().text(data.workerName); // 작업자명
				break;
			default:
				break;
		}
	});

	// 팝업 표시
	$(".productHistoryViewPopup").removeClass("hide");
}


async function loadMoveFileHistoryTable() {

	var searchDTO = {
		dateSelect: $(".moveFileHistorySearchPop .search_wrap select[name='dateSelectPopMfile']").val(),
		startDt: $(".moveFileHistorySearchPop .search_wrap input[name='startDt']").val(),
		endDt: $(".moveFileHistorySearchPop .search_wrap input[name='endDt']").val(),
		status: $(".moveFileHistorySearchPop .search_wrap select[name='status']").val(),
		condition: $(".moveFileHistorySearchPop .search_wrap select[name='condition']").val(),
		searchKeyword: $(".moveFileHistorySearchPop .search_wrap input[name='searchKeyword']").val()
	};
	
	moveFileHistoryTable = await initDataTables('#moveFileHistoryTable', {
		ajax: {
			url: '/primx/fun/file/loadMoveFileHistory.do',
			type: 'POST',
			data: searchDTO,
			dataSrc: function (json) {
				// 일련번호 생성
				var data = json.data;
				for (var i = 0; i < data.length; i++) {
					var idx = json.recordsTotal - ((json.page-1) * json.pageSize) - i;
					data[i].rowId = idx;
				}
				return data;
			}
		},
		processing: true,
    	serverSide: true,
		columns: [
			// 번호 파일id 상태 파일명 사이즈 원본파일수 잔여횟수 시스템명 사용자 등록일시 종료일\
			{ data: null},
			{ data: 'rowId' },
			{ data: 'fileKey' },
			// 상태는 expireDate가 현재시간보다 크면 사용, 작으면 종료
			{ data: 'expireDate', render: function(data, type, row, meta) {
				return compareDate(data);
			}},
			{ data: 'fileName' },
			{ data: 'fileSize', render: function(data, type, row, meta) {
				return convertByteUnitWithComma(data, 'B');
			}},
			{ data: 'fileMapCount' },
			{ data: 'decryptCnt' },
			{ data: 'computerName' },
			{ data: 'userNm' },
			{ data: 'insertDate' },
			{ data: 'expireDate' }
		],
		columnDefs: [
			{
				render: DataTable.render.select(),
				targets: 0
			}
		],
		buttons: [
			{
				extend: 'csv',
				title: `${firstDepthTit}_${secondDepthTit}_${formattedDate}`,
				exportOptions: {
					columns: ':not(:first-child)'  // 첫 번째 컬럼(체크박스)을 제외한 모든 컬럼 내보내기
				},
				bom: true, // UTF-8 BOM 추가
				customize: function(csv) {
					return '\ufeff' + csv; // UTF-8 BOM 추가하여 한글 깨짐 방지
				}
			}
		],
		select: {
			style: 'multi',
			selector: 'td:first-child'
		}
	});
	
	// 테이블 데이터 클릭 시 팝업 오픈 함수 호출(해당 row전달)
	$('#moveFileHistoryTable tbody').on("click", 'td:not(:first-child)', function(){
		var rowData = moveFileHistoryTable.row(this).data();
		openMoveFileHistoryViewPopup(rowData);
	})

	// CSV 내보내기 버튼과 연결
	$('#moveFileHistoryCsvExport').on('click', function() {
		moveFileHistoryTable.button('.buttons-csv').trigger();
	});
}


function openMoveFileHistoryViewPopup(rowData) {
	console.log(rowData);
	initMoveFileHistoryViewPopup();
	$(".moveFileHistoryViewPopup .pop_cont [name='insertDate']").text(rowData.insertDate);
	$(".moveFileHistoryViewPopup .pop_cont [name='status']").text(compareDate(rowData.expireDate));
	$(".moveFileHistoryViewPopup .pop_cont [name='computerName']").text(rowData.computerName);
	$(".moveFileHistoryViewPopup .pop_cont [name='userNm']").text(rowData.userNm);
	$(".moveFileHistoryViewPopup .pop_cont [name='fileKey']").text(rowData.fileKey);
	$(".moveFileHistoryViewPopup .pop_cont [name='fileName']").text(rowData.fileName);
	$(".moveFileHistoryViewPopup .pop_cont [name='decryptCnt']").text(rowData.decryptCnt);
	$(".moveFileHistoryViewPopup .pop_cont [name='expireDate']").text(rowData.expireDate);

	// 팝업 내 원본파일 조회
	loadOriginalFileInfoTable(rowData.fileKey);

	// 팝업 내 이동파일 사용 이력 조회
	loadMoveFileUsageHistoryTable(rowData.fileKey);

	// 팝업 내 탭 초기화
	$(".moveFileTabWrap .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
	$(".moveFileTabWrap .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
	$('.moveFileTabWrap .tabTitle li:first-child').addClass("on"); //첫번째 li에 테두리를 추가해라
	$('.moveFileTabWrap .tabCont > li:first-child').addClass("on"); //첫번째 탭을 선택해라

	$(".moveFileHistoryViewPopup").removeClass("hide");
}

// 상세 팝업 초기화
function initMoveFileHistoryViewPopup() {
	$(".moveFileHistoryViewPopup .pop_cont [name='insertDate']").text("");
	$(".moveFileHistoryViewPopup .pop_cont [name='status']").text("");
	$(".moveFileHistoryViewPopup .pop_cont [name='computerName']").text("");
	$(".moveFileHistoryViewPopup .pop_cont [name='userNm']").text("");
	$(".moveFileHistoryViewPopup .pop_cont [name='fileKey']").text("");
	$(".moveFileHistoryViewPopup .pop_cont [name='fileName']").text("");
	$(".moveFileHistoryViewPopup .pop_cont [name='decryptCnt']").text("");
	$(".moveFileHistoryViewPopup .pop_cont [name='expireDate']").text("");

	originalFileInfoTable = null;
	moveFileUsageHistoryTable = null;
}

// 날짜 비교 결과 리턴
function compareDate(data) {
	var expireDate = moment(data, 'YYYY-MM-DD HH:mm:ss').toDate();
	var currentDate = new Date();

	console.log('data: ' + data);
	console.log('new Date(): ' + currentDate);

	// 날짜 비교
	return expireDate > currentDate ? '사용' : '종료';
}

// 원본파일 조회
function loadOriginalFileInfoTable(fileKey) {
	console.log('fileKey: ' + fileKey);

	originalFileInfoTable = initDataTables('#originalFileInfoTable', {
		ajax: {
			url: '/primx/fun/file/loadOriginalFileInfo.do',
			type: 'POST',
			data: { fileKey: fileKey },
			dataSrc: 'originalFileInfoList'
		}, 
		columns: [
			{ data: null, render: function(data, type, row, meta) {
				return meta.row + 1; // 행 번호 자동 생성
			}},
			{ data: 'fileName' },
			{ data: 'fileSize', render: function(data, type, row, meta) {
				return convertByteUnitWithComma(data);
			}}
		]
	});
}

// 이동파일 사용 이력 조회
function loadMoveFileUsageHistoryTable(fileKey) {
	console.log('fileKey: ' + fileKey);

	// 기존 테이블이 있다면 제거
	if (moveFileUsageHistoryTable) {
		moveFileUsageHistoryTable.destroy();
		moveFileUsageHistoryTable = null;
	}

	// 기존 CSV 버튼 이벤트 바인딩 제거
	$('#moveFileUsageHistoryPopupCsvExport').off('click');

	moveFileUsageHistoryTable = initDataTables('#moveFileUsageHistoryTable', {
		ajax: {
			url: '/primx/fun/file/loadMoveFileUsageHistory.do',
			type: 'POST',
			data: { fileKey: fileKey },
			dataSrc: 'moveFileUsageHistoryList'
		}, 
		columns: [
			// 번호 사용일자 사용시각 인증요청IP 종료일 잔여횟수 결과 거부사유
			{ data: null, render: function(data, type, row, meta) {
				return meta.row + 1; // 행 번호 자동 생성
			}},
			// 사용일자는 logTime의 년월일
			{ data: 'logTime', render: function(data) {
				return moment(data).format('YYYY-MM-DD');
			}},
			// 사용시각은 logTime의 시간
			{ data: 'logTime', render: function(data) {
				return moment(data).format('HH:mm:ss');
			}},
			{ data: 'requestIp' },
			{ data: 'userName' },
			{ data: 'expireDate', render: function(data) {
				return moment(data).format('YYYY-MM-DD');
			}},
			{ data: 'decryptCnt' },
			{ data: 'resultCode' },
			{ data: 'comment' }
		]
	});
}

function searchSystemStatus(popupName, tableSearchKeyword = null) {

    if(popupName == 'searchAreaPopup') {
		var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
		var searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
		var searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
		var searchLastKaDateType = $(".searchAreaPopup .search_wrap select[name='lastKaDateType']");
		var searchCommonSystemYn = $(".searchAreaPopup .search_wrap select[name='commonSystemYn']");
		var searchProductName = $(".searchAreaPopup .search_wrap select[name='productName']");
		var searchType = $(".searchAreaPopup .search_wrap select[name='searchType']");
		var searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']");
		
		if(searchDateType.val() != null && searchDateType.val() != "") {
			
			if(!searchFieldValid('date', searchStartDate.val(), searchEndDate.val(), searchDateType.val())) {
				return;
			}
			var filterStr = searchStartDate.val() +' ~ '+ searchEndDate.val();
			addFilterItem(".systemStatus .searchBox", "기간", filterStr, popupName);
		}
		else {
			resetSearchValue(popupName, "기간");
		}
		
		if(searchLastKaDateType.val() != null && searchLastKaDateType.val() != "") {
			addFilterItem(".systemStatus .searchBox", "미접속 기간", searchLastKaDateType.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "미접속 기간");
		}

		if(searchCommonSystemYn.val() != null && searchCommonSystemYn.val() != "") {
			addFilterItem(".systemStatus .searchBox", "시스템 종류", searchCommonSystemYn.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "시스템 종류");
		}

		if(searchProductName.val() != null && searchProductName.val() != "") {
			addFilterItem(".systemStatus .searchBox", "설치 제품", searchProductName.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "설치 제품");
		}

		if(searchType.val() != null && searchType.val() != "" && searchKeyword.val() != null && searchKeyword.val() != "") {
			if(!searchFieldValid('condition', searchKeyword.val(), null, null)) {
				return;
			}
			addFilterItem(".systemStatus .searchBox", "조건", searchType.find('option:selected').text() + " [" + searchKeyword.val() + "]", popupName);
		}
		else {
			resetSearchValue(popupName, "조건");
		}
		fnSearchList();
		/* 검색 팝업 닫기 */
		$(".searchAreaPopup").addClass("hide");
	}
	else if(popupName == 'moveFileHistorySearchPop') {
		var searchDateType = $(".moveFileHistorySearchPop .search_wrap select[name='dateSelectPopMfile']");
		var searchStartDate = $(".moveFileHistorySearchPop .search_wrap input[name='startDt']");
		var searchEndDate = $(".moveFileHistorySearchPop .search_wrap input[name='endDt']");
		var searchStatus = $(".moveFileHistorySearchPop .search_wrap select[name='status']");
		var searchCondition = $(".moveFileHistorySearchPop .search_wrap select[name='condition']");
		var searchKeyword = $(".moveFileHistorySearchPop .search_wrap input[name='searchKeyword']");
		
		if(tableSearchKeyword != null && tableSearchKeyword != "") {
			searchCondition.val('user');
			searchKeyword.val(tableSearchKeyword);
		}

		if(searchDateType.val() != null && searchDateType.val() != "") {
			if(!searchFieldValid('date', searchStartDate.val(), searchEndDate.val(), searchDateType.val())) {
				return;
			}
			var filterStr = searchStartDate.val() +' ~ '+ searchEndDate.val();
			addFilterItem(".moveFileHistoryPopup .searchBox", "기간", filterStr, popupName);
		}
		else {
			resetSearchValue(popupName, "기간");
		}

		if(searchStatus.val() != null && searchStatus.val() != "" ) {
			addFilterItem(".moveFileHistoryPopup .searchBox", "상태", searchStatus.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "상태");
		}

		if(searchCondition.val() != null && searchCondition.val() != "" && searchKeyword.val() != null && searchKeyword.val() != "") {
			if(!searchFieldValid('condition', searchKeyword.val(), null, null)) {
				return;
			}
			addFilterItem(".moveFileHistoryPopup .searchBox", "조건", searchCondition.find('option:selected').text() + " [" + searchKeyword.val() + "]", popupName);
		}
		else {
			resetSearchValue(popupName, "조건");
		}
		loadMoveFileHistoryTable();
		/* 검색 팝업 닫기 */
		$(".moveFileHistorySearchPop").addClass("hide");
		
	}
	else if(popupName == 'productHistorySearchPop') {
		var searchDateType = $(".productHistorySearchPop .search_wrap select[name='dateSelectPopProduct']");
		var searchStartDate = $(".productHistorySearchPop .search_wrap input[name='startDt']");
		var searchEndDate = $(".productHistorySearchPop .search_wrap input[name='endDt']");
		var searchLevel = $(".productHistorySearchPop .search_wrap select[name='searchLevel']");
		var searchProduct = $(".productHistorySearchPop .search_wrap select[name='searchProduct']");
		var searchType = $(".productHistorySearchPop .search_wrap select[name='searchType']");
		var searchCondition = $(".productHistorySearchPop .search_wrap select[name='condition']");
		var searchKeyword = $(".productHistorySearchPop .search_wrap input[name='searchKeyword']");

		if(tableSearchKeyword != null && tableSearchKeyword != "") {
			searchCondition.val('userName');
			searchKeyword.val(tableSearchKeyword);
		}

		if(searchDateType.val() != null && searchDateType.val() != "") {
			if(!searchFieldValid('date', searchStartDate.val(), searchEndDate.val(), searchDateType.val())) {
				return;
			}
			var filterStr = searchStartDate.val() +' ~ '+ searchEndDate.val();
			addFilterItem(".productHistoryPopup .searchBox", "기간", filterStr, popupName);
		}
		else {
			resetSearchValue(popupName, "기간");
		}

		if(searchLevel.val() != null && searchLevel.val() != "") {
			addFilterItem(".productHistoryPopup .searchBox", "레벨", searchLevel.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "레벨");
		}
		
		if(searchProduct.val() != null && searchProduct.val() != "") {
			addFilterItem(".productHistoryPopup .searchBox", "제품", searchProduct.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "제품");
		}

		if(searchType.val() != null && searchType.val() != "") {
			addFilterItem(".productHistoryPopup .searchBox", "종류", searchType.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "종류");
		}

		if(searchCondition.val() != null && searchCondition.val() != "" && searchKeyword.val() != null && searchKeyword.val() != "") {
			if(!searchFieldValid('condition', searchKeyword.val(), null, null)) {
				return;
			}
			addFilterItem(".productHistoryPopup .searchBox", "조건", searchCondition.find('option:selected').text() + " [" + searchKeyword.val() + "]", popupName);
		}
		else {
			resetSearchValue(popupName, "조건");
		}

		initProductHistoryDataTable();
		/* 검색 팝업 닫기 */
		$(".productHistorySearchPop").addClass("hide");

	}
}

function removeFilter(element, popupName) {

	// 선택된 필터의 filterTitle을 찾아서 초기화함수 호출
	var filterTitle = $(element).closest('.filterItem').find('.filterTitle').text();
	resetSearchValue(popupName, filterTitle);
	$(element).closest('.filterItem').remove(); // 해당 필터 아이템 제거

    if(popupName == 'searchAreaPopup') {
		fnSearchList();
    }
	else if(popupName == 'moveFileHistorySearchPop') {
		loadMoveFileHistoryTable();
	}
	else if(popupName == 'productHistorySearchPop') {
		initProductHistoryDataTable();
	}
    else {
        console.log('removeFilter: no match popupName: ', popupName);
    }
}

function resetSearchValue(popupName, filterTitle) {
    console.log('resetSearchValue: ', filterTitle);

    if(popupName == 'searchAreaPopup') {
		var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
		var searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
		var searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
		var searchLastKaDateType = $(".searchAreaPopup .search_wrap select[name='lastKaDateType']");
		var searchCommonSystemYn = $(".searchAreaPopup .search_wrap select[name='commonSystemYn']");
		var searchProductName = $(".searchAreaPopup .search_wrap select[name='productName']");
		var searchType = $(".searchAreaPopup .search_wrap select[name='searchType']");
		var searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']");
		
		if(filterTitle == "기간") {
			searchDateType.val(searchDateType.find('option[value=""]').val());
			searchStartDate.val('');
			searchEndDate.val('');
		}   
		else if(filterTitle == "미접속 기간") {
			searchLastKaDateType.val(searchLastKaDateType.find('option[value=""]').val());
		}
		else if(filterTitle == "시스템 종류") {
			searchCommonSystemYn.val(searchCommonSystemYn.find('option[value=""]').val());
		}
		else if(filterTitle == "설치 제품") {
			searchProductName.val(searchProductName.find('option[value=""]').val());
		}
		else if(filterTitle == "조건") {
			searchType.val(searchType.find('option[value="all"]').val());
			searchKeyword.val('');
		}
	}
	else if(popupName == 'moveFileHistorySearchPop') {
		var searchDateType = $(".moveFileHistorySearchPop .search_wrap select[name='dateSelectPopMfile']");
		var searchStartDate = $(".moveFileHistorySearchPop .search_wrap input[name='startDt']");
		var searchEndDate = $(".moveFileHistorySearchPop .search_wrap input[name='endDt']");
		var searchStatus = $(".moveFileHistorySearchPop .search_wrap select[name='status']");
		var searchCondition = $(".moveFileHistorySearchPop .search_wrap select[name='condition']");
		var searchKeyword = $(".moveFileHistorySearchPop .search_wrap input[name='searchKeyword']");
		
		if(filterTitle == "기간") {
			searchDateType.val(searchDateType.find('option[value=""]').val());
			searchStartDate.val('');
			searchEndDate.val('');
		}   
		else if(filterTitle == "상태") {
			searchStatus.val(searchStatus.find('option[value=""]').val());
		}
		else if(filterTitle == "조건") {
			searchCondition.val(searchCondition.find('option[value="all"]').val());
			searchKeyword.val('');
		}
	}
	else if(popupName == 'productHistorySearchPop') {
		var searchDateType = $(".productHistorySearchPop .search_wrap select[name='dateSelectPopProduct']");
		var searchStartDate = $(".productHistorySearchPop .search_wrap input[name='startDt']");
		var searchEndDate = $(".productHistorySearchPop .search_wrap input[name='endDt']");
		var searchLevel = $(".productHistorySearchPop .search_wrap select[name='searchLevel']");
		var searchProduct = $(".productHistorySearchPop .search_wrap select[name='searchProduct']");
		var searchType = $(".productHistorySearchPop .search_wrap select[name='searchType']");
		var searchCondition = $(".productHistorySearchPop .search_wrap select[name='condition']");
		var searchKeyword = $(".productHistorySearchPop .search_wrap input[name='searchKeyword']");
		
		if(filterTitle == "기간") {
			searchDateType.val(searchDateType.find('option[value=""]').val());
			searchStartDate.val('');
			searchEndDate.val('');
		}
		else if(filterTitle == "레벨") {
			searchLevel.val(searchLevel.find('option[value=""]').val());
		}
		else if(filterTitle == "제품") {
			searchProduct.val(searchProduct.find('option[value=""]').val());
		}
		else if(filterTitle == "종류") {
			searchType.val(searchType.find('option[value=""]').val());
		}
		else if(filterTitle == "조건") {
			searchCondition.val(searchCondition.find('option[value="all"]').val());
			searchKeyword.val('');
		}
	}
    
}


function updateSystemType(data) {
	$('#sysTypeSelectRadio_Y').prop('checked', false);
	$('#sysTypeSelectRadio_N').prop('checked', false);

	console.log(data);
	if(data.commonSystemYn == 'Y') {
		$('#sysTypeSelectRadio_Y').prop('checked', true);
	} else {
		$('#sysTypeSelectRadio_N').prop('checked', true);
	}
	console.log($('#sysTypeSelectRadio_Y').is(':checked'));
	console.log($('#sysTypeSelectRadio_N').is(':checked'));
	$('.changeSystemTypePopup').removeClass('hide');
}

function fnChangeSystemType() {
    // 선택된 row의 데이터 정보를 가져옴
    var selectedRow = table.rows({ selected: true }).data();
    
    // selectedRow의 내용을 개별적으로 출력
    selectedRow.each(function(row) {
        console.log('Row data:', row); // 각 행의 데이터 출력
    });

    // sysTypeSelectRadio_Y 체크된 값 가져오기
    var selectedSystemType;

    // $('#sysTypeSelectRadio_Y') 이게 선택되었는지 확인
    if ($('#sysTypeSelectRadio_Y').is(':checked')) {
        selectedSystemType = 'Y';
    } else if ($('#sysTypeSelectRadio_N').is(':checked')) {
        selectedSystemType = 'N';
    }

    console.log('selectedSystemType: ' + selectedSystemType);


	$.ajax({
		url: '/primx/fun/sys/updateSystemType.do',
		type: 'POST',
		data: {
			seqNo: selectedRow[0].systemId,
			agentId: selectedRow[0].agentId,
			userSystemMatchSeqNo: selectedRow[0].userSystemMatchSeqNo,
			commonSystemYn: selectedSystemType
		},
		success: function(response) {
			console.log(response);
			fnSearchList();
		},
		error: function(error) {
			console.log(error);
		}
	});
	
	$('.changeSystemTypePopup').addClass('hide');
}

// 시스템 삭제 팝업 
function fnSysDel() {
	// 선택된 row의 데이터 정보를 가져옴
    var selectedRow = table.rows({ selected: true }).data();

    // selectedRow의 내용을 개별적으로 출력
    selectedRow.each(function(row) {
        console.log('Row data:', row); // 각 행의 데이터 출력
    });

	$.ajax({
		url: '/primx/fun/sys/deleteSystem.do',
		type: 'POST',
		data: {
			seqNo: selectedRow[0].systemId
		},
		success: function(response) {
			console.log(response);
			alert('시스템이 삭제 처리 되었습니다.');
			fnSearchList();
		},
		error: function(error) {
			console.log(error);
			alert('시스템 삭제 처리 중 오류가 발생했습니다.');
		}
	});
    // sysDelPopup 팝업 표시
    $('.sysDelPopup').addClass('hide');
}

function fnSysDelCancel() {
	$('.sysDelPopup').addClass('hide');
}

function changePolicy(data) {
	console.log(data);

	// 기본값 
	loadProductPolicy('ZONECENTRAL', data);

	// 제품 핸들러 등록
	productHandler(data);

	// 
	$('.policyChangePopup').removeClass('hide');
}


function loadProductPolicyTable() {
	var data = table.rows({ selected: true }).data();
	if($('.policyChangePopup select').val() == 1) {
		loadProductPolicy('ZONECENTRAL', data);
		$('.zonecentralArea').removeClass('hide');
		$('.orizonArea').addClass('hide');
	} else {
		loadProductPolicy('ORIZON', data);
		$('.zonecentralArea').addClass('hide');
		$('.orizonArea').removeClass('hide');
	}
}

function productHandler(data) {
	// policyChangePopup selectBox 값이 변경되면, 값에 따라 1: zonecentralArea, 2: orizonArea 표시
	$('.policyChangePopup select').on('change', function() {
		if($('.policyChangePopup select').val() == 1) {
			loadProductPolicy('ZONECENTRAL', data);
			$('.zonecentralArea').removeClass('hide');
			$('.orizonArea').addClass('hide');
		} else {
			loadProductPolicy('ORIZON', data);
			$('.zonecentralArea').addClass('hide');
			$('.orizonArea').removeClass('hide');
		}
	});
}

async function loadProductPolicy(product, data) {
	console.log('product: ' + product);
	console.log('data: ' +  data);
	console.log('data.length: ' + data.length);

	var tableId = null;
	if(product == null) {
		alert('제품을 선택해주세요.');
		return;
	}
	if(product == 'ZONECENTRAL') {
		tableId = '#zonecentralPolicyTable';
	} else {
		tableId = '#orizonPolicyTable';
	}
	console.log('tableId: ' + tableId);
	console.log('product: ' + product);

	// 선택된 모든 data.agentId 가져오기, 구분자 ',' 로 구분
	var agentIdList = [];
	for(var i = 0; i < data.length; i++) {
		agentIdList.push(data[i].agentId);
	}
	console.log('agentIdList: ' + agentIdList);

	var policyCodeList = [];
	
	if(data.length > 0) {
		// data 배열 순회
		for(var i = 0; i < data.length; i++) {
			var singleData = data[i];
			// singleData의 policys가 있고 데이터가 없거나 null이 아니면 
			if(singleData.policys && singleData.policys != null) {
				// singleData의 policys를 || 로 구분된 값을 배열로 변환
				var policys = singleData.policys.split('||');
				policyCodeList.push(policys[0]);
			}
		}
	}
	
	console.log('policyCodeList: ' + policyCodeList);
	// policyCodeList에 중복된 값 제거
	policyCodeList = [...new Set(policyCodeList)];
	console.log('policyCodeList: ' + policyCodeList);

	productPolicyTable = await initDataTables(tableId ,{
		ajax: {
			url: '/primx/fun/pol/product/policy/list.do',
			type: 'POST',
			dataSrc: 'resultList',
			data: function(d) {
				d.searchProduct = product;
				d.searchCondition = 'policyName';
				d.searchKeyword = $(".policyChangePopup .stickyWrap input[name='searchKeyword']").val();
			}
		},
		columns: [
			{ 
				data: null, 
				orderable: false, // 정렬 비활성화
				className: "text-center",
				render: function(data, type, row, meta) {
					if (type === "display") {
						return `<input type="checkbox" name="rowSelect" class="row-checkbox" />`;
					}
					return "";
				}
			},
			{ data: null, render: function(data, type, row, meta) {
				return meta.row + 1; // 행 번호 자동 생성
			}},
			{ data: 'policyCode' },
			{ data: 'productName' },
			{ data: 'policyName' },
			// td class=result 로 설정
			{ data: function(data, type, row, meta) {
				if(data.isDefault == 'Y') {
					return '<span class="defaultPol">기본</span>';
				} else {
					return '<span class="eachPol">개별</span>';
				}
				},
				createdCell: function(td, cellData, rowData, row, col) {
					$(td).addClass('result');
				}
			},
			{ data: 'systemMapCount' },
			{ data: 'createdByNm' },
			{ data: 'createdDate', render: function(data, type, row, meta) {
				return moment(data).format('YYYY-MM-DD');
			}},
			{ data: 'modifiedDate', render: function(data, type, row, meta) {
				return moment(data).format('YYYY-MM-DD');
			}}
		],
		columnDefs: [
			{
				targets: 0, // 첫 번째 열
				orderable: false, // 정렬 비활성화
				className: "text-center"
			}
		],
		select: {
			style: 'single',
			selector: 'input.row-checkbox'
		}
	});

	// 단일 선택 체크박스 이벤트
	$(tableId + ' tbody').on('change', '.row-checkbox', function() {
	    $('.row-checkbox').not(this).prop('checked', false); // 다른 체크박스 해제
	    $(tableId + ' tbody tr').removeClass('selected'); // 모든 행에서 선택 효과 제거
	
	    if ($(this).prop('checked')) {
	        $(this).closest('tr').addClass('selected'); // 선택된 행에 배경색 적용
	    }
	});

	productPolicyTable.on('draw', function () {
	    var tableSize = table.rows().count();
	    console.log('tableSize: ' + tableSize);
	
	    // 테이블 데이터 순회하여 policyCodeList와 비교 후 .disTr 추가
	    productPolicyTable.rows().every(function(rowIdx, tableLoop, rowLoop) {
	        var rowData = this.data(); // DataTables에서 행 데이터 가져오기
	
	        if (rowData && rowData.policyCode && policyCodeList.includes(rowData.policyCode)) {
	            $(this.node()).addClass('disTr'); // tr에 disTr 클래스 추가
	        }
			else {
				$(this.node()).removeClass('disTr'); // tr에 disTr 클래스 제거
			}
	    });
	});

	// 핸들러 추가, row 선택 시
	$(tableId + ' tbody').on('click', 'td:not(:first-child)', function() {
		// 선택된 rowData.policyCode 를 가져옴
		var rowData = productPolicyTable.row(this).data();
		var policyCode = rowData.policyCode;
		console.log('policyCode: ' + policyCode);
		console.log('agentIdList: ' + agentIdList);
	});
}

function changeProductPolicy() {
	console.log('changeProductPolicy');

	var data = table.rows({ selected: true }).data();

	// 선택된 모든 data.agentId 가져오기, 구분자 ',' 로 구분
	var agentIdList = [];
	for(var i = 0; i < data.length; i++) {
		agentIdList.push(data[i].agentId);
	}
	console.log('agentIdList: ' + agentIdList);

	// productPolicyTable 에서 선택된 정보 가져오기
	var rowData = productPolicyTable.rows({ selected: true }).data();
	
	// 선택된 코드가 없으면 경고창 표시
	if(rowData.length == 0) {
		alert('정책을 선택해주세요.');
		return;
	}

	console.log('Sending request with:', {
		agentIdList: agentIdList.join(','),  // 배열을 쉼표로 구분된 문자열로 변환
		policyCode: rowData[0].policyCode
	});
	
	$.ajax({
		url: '/primx/fun/pol/product/changePolicy.do',  // URL은 동일
		type: 'POST',
		data: {
			agentIdList: agentIdList.join(','),  // 배열을 쉼표로 구분된 문자열로 변환
			policyCode: rowData[0].policyCode
		},
		success: function(response) {
			console.log('Response received:', response);
			if(response.status == 'success') {
				console.log(response);
				alert('정책 변경 처리 되었습니다.');
				fnSearchList();
				$('.policyChangePopup').addClass('hide');
			} else {
				alert('정책 변경 처리 중 오류가 발생했습니다.');
			}
		},
		error: function(error) {
			console.error('Error details:', error);
			alert('정책 변경 처리 중 오류가 발생했습니다.');
		}
	});
}


function loadAgentPolicyTable() {
	var data = table.rows({ selected: true }).data();
	changeAgentPolicy(data);
}

async function changeAgentPolicy(data) {

	console.log(data);
	
	// 선택된 모든 data.agentId 가져오기, 구분자 ',' 로 구분
	var agentIdList = [];
	for(var i = 0; i < data.length; i++) {
		agentIdList.push(data[i].agentId);
	}
	console.log('agentIdList: ' + agentIdList);

	var agentPolicyCodeList = [];
	
	if(data.length > 0) {
		// data 배열 순회
		for(var i = 0; i < data.length; i++) {
			var singleData = data[i];
			// singleData의 policys가 있고 데이터가 없거나 null이 아니면 
			if(singleData.agentPolicys && singleData.agentPolicys != null) {
				// singleData의 policys를 || 로 구분된 값을 배열로 변환
				var policys = singleData.agentPolicys.split('||');
				agentPolicyCodeList.push(policys[0]);
			}
		}
	}
	
	console.log('agentPolicyCodeList: ' + agentPolicyCodeList);
	// agentPolicyCodeList에 중복된 값 제거
	agentPolicyCodeList = [...new Set(agentPolicyCodeList)];
	console.log('agentPolicyCodeList: ' + agentPolicyCodeList);




	// 모든 정책 리스트 가져오기
	agentPolicyTable = await initDataTables('#agentPolicyTable', {
		ajax: {
			url: '/primx/fun/pol/agent/policy/loadAgentPolicy.do',
			type: 'POST',
			data: {
				policyName: $(".agentChangePopup .stickyWrap input[name='searchKeyword']").val()
			},
			dataSrc: 'agentPolicyList'
		}, 
		columns: [
			// 선택, 번호, 정책ID, 정책명, 유형, 적용, 등록자, 등록일, 수정일
			{ 
				data: null, 
				orderable: false, // 정렬 비활성화
				className: "text-center",
				render: function(data, type, row, meta) {
					if (type === "display") {
						return `<input type="checkbox" name="rowSelect" class="row-checkbox" />`;
					}
					return "";
				}
			},
			{ data: null, render: function(data, type, row, meta) {
				return meta.row + 1; // 행 번호 자동 생성
			}},
			{ data: 'agentPolicyCode' },
			{ data: 'policyName' },
			{ data: function(data, type, row, meta) {
				if(data.defaultPolicyYn == 'Y') {
					return '<span class="defaultPol">기본</span>';
				} else {
					return '<span class="eachPol">개별</span>';
				}
				},
				createdCell: function(td, cellData, rowData, row, col) {
					$(td).addClass('result');
				}
			},
			{ data: 'systemMapCount' },
			{ data: 'insertedBy' },
			{ data: 'insertDate', render: function(data, type, row, meta) {
				return moment(data).format('YYYY-MM-DD');
			}},
			{ data: 'updateDate', render: function(data, type, row, meta) {
				return moment(data).format('YYYY-MM-DD');
			}}
		],
		columnDefs: [
			{
				targets: 0, // 첫 번째 열
				orderable: false, // 정렬 비활성화
				className: "text-center"
			}
		],
		select: {
			style: 'single',
			selector: 'input.row-checkbox'
		}
	});

	// 단일 선택 체크박스 이벤트
	$('#agentPolicyTable tbody').on('change', '.row-checkbox', function() {
	    $('.row-checkbox').not(this).prop('checked', false); // 다른 체크박스 해제
	    $('#agentPolicyTable tbody tr').removeClass('selected'); // 모든 행에서 선택 효과 제거
	
	    if ($(this).prop('checked')) {
	        $(this).closest('tr').addClass('selected'); // 선택된 행에 배경색 적용
	    }
	});
	
	agentPolicyTable.on('draw', function () {
	    var tableSize = table.rows().count();
	    console.log('tableSize: ' + tableSize);
	
	    // 테이블 데이터 순회하여 policyCodeList와 비교 후 .disTr 추가
	    agentPolicyTable.rows().every(function(rowIdx, tableLoop, rowLoop) {
	        var rowData = this.data(); // DataTables에서 행 데이터 가져오기
			
	        if (rowData && rowData.agentPolicyCode && agentPolicyCodeList.includes(rowData.agentPolicyCode)) {
	            $(this.node()).addClass('disTr'); // tr에 disTr 클래스 추가
	        }
			else {
				$(this.node()).removeClass('disTr'); // tr에 disTr 클래스 제거
			}
	    });
	});
	
	// 핸들러 추가, row 선택 시
	$('#agentPolicyTable tbody').on('click', 'td:not(:first-child)', function() {
		// 선택된 rowData.policyCode 를 가져옴
		var rowData = agentPolicyTable.row(this).data();
		var agentPolicyCode = rowData.agentPolicyCode;
		console.log('agentPolicyCode: ' + agentPolicyCode);
		console.log('agentIdList: ' + agentIdList);
	});

	$('.agentChangePopup').removeClass('hide');
}



function addAgentSystem(){
	
	var data = table.rows({ selected: true }).data();

	// 선택된 모든 data.agentId 가져오기, 구분자 ',' 로 구분
	var agentIdList = [];
	for(var i = 0; i < data.length; i++) {
		agentIdList.push(data[i].agentId);
	}
	console.log('agentIdList: ' + agentIdList);

    // agentPolicyTable 테이블의 선택된 row의 agentPolicyCode 가져오기
    var selectedPolicyRow = $('#agentPolicyTable').DataTable().rows({ selected: true }).data();
    var agentPolicyCode = selectedPolicyRow[0].agentPolicyCode;

	// 선택된 코드가 없으면 경고창 표시
	if(selectedPolicyRow.length == 0) {
		alert('Agent 정책을 선택해주세요.');
		return;
	}

    console.log('agentIdList: ' + agentIdList);
    console.log('agentPolicyCode: ' + agentPolicyCode);

	
    $.ajax({
        url: '/primx/fun/pol/agent/policy/changeSystem.do',
        type: 'POST',
        data: {
			agentIdList: agentIdList.join(','),  // 배열을 쉼표로 구분된 문자열로 변환
            agentPolicyCode: agentPolicyCode
        },
        success: function(data){
            console.log('data: ', data);
            if(data.status === "success"){
                alert('Agent 정책 변경 성공');
                fnSearchList();
                $('.agentChangePopup').addClass('hide');
            } else {
                alert('Agent 정책 변경 실패: ' + data.message);
            }
        },
        error: function(xhr, status, error){
            console.error('error: ', error);
        }
    });
	
}

function openProductHistorySearchPopup() {
	$('.productHistorySearchPop').removeClass('hide');
	$('.productHistorySearchPop #searchKeyword').focus();
}

function openMoveFileHistorySearchPopup() {
	$('.moveFileHistorySearchPop').removeClass('hide');
	$('.moveFileHistorySearchPop #searchKeyword').focus();
}

