let dataTable;
let fileZoneTable;
let polSysStatusTable;
let agnSysStatusAddTable;

/* csv 다운로드 제목포멧 */
var firstDepthTit = $(".left nav .firstDepthMenu > li.select > p").text();
var secondDepthTit = $(".left nav .firstDepthMenu > li.select .secondDepthMenu > li.select a").text();
var today = new Date();
var formattedDate = today.getFullYear() + '-' +
    ('0' + (today.getMonth() + 1)).slice(-2) + '-' +
    ('0' + today.getDate()).slice(-2);
    
/* 정책 > Agent정책 */
function pageInit(){
	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();

	/* input file 커스텀 */
	inputFileCustom();
	/* dataTables */
	dataTable = initDataTables("#agentStatusTable");
	initializeDataTable();
	/* datepicker */
	datePicker();

}

function initializeDataTable(){
	// 기존 테이블이 있다면 제거
	if (dataTable) {
		dataTable.destroy();
		dataTable = null;
	}

	// 기존 CSV 버튼 이벤트 바인딩 제거
	$('#agentStatusCsvExport').off('click');

	dataTable = initDataTables("#agentStatusTable", {
		processing: true,
		serverSide: true,
		ajax: {
			url: '/primx/fun/pol/agent/policy/list.do',
			type: 'POST',
			dataSrc: 'resultList',
			data: function(d) {
				d.searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']").val();
				d.searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']").val();
				d.searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']").val();
				d.searchPolicyType = $(".searchAreaPopup .search_wrap select[name='searchPolicyType']").val();
				d.searchCondition = $(".searchAreaPopup .search_wrap select[name='searchCondition']").val();
				d.searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']").val();
			}
		},
		columns: [
			{ 
				data: null, 
				orderable: false, // 정렬 비활성화
				className: "text-center",
				render: function(data, type, row, meta) {
					if (type === "display") {
						return `<input type="checkbox" name="rowSelect" value="${row.agentPolicyCode}" class="row-checkbox" />`;
					}
					return "";
				}
			},
			{ data: null, render: function(data, type, row, meta) {
				return meta.row + 1; // 행 번호 자동 생성
			}},
			{
				data: "defaultPolicyYn",
				render: function(data, type, row) {
					return data === 'Y' ? '<span class="defaultPol">기본</span>' : '<span class="eachPol">개별</span>';
				},
				createdCell: function (td, cellData, rowData, row, col) {
			        $(td).addClass('result');
			    }
			},
			{ data: "agentPolicyCode" },
			{ data: "policyName" },
			{
				data: "systemMapCount",
			},
			{ data: "insertedBy" },
			{ 
				data: "insertDate",
				render: function(data) {
					return new Date(data).toLocaleString();
				}
			},
			{ 
				data: "updateDate",
				render: function(data) {
					return new Date(data).toLocaleString();
				}
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
		columnDefs: [
			{
				targets: 0, // 첫 번째 열
				orderable: false, // 정렬 비활성화
				className: "text-center"
			}
		]
	});

	// CSV 내보내기 버튼과 연결
	$('#agentStatusCsvExport').on('click', function() {
		dataTable.button('.buttons-csv').trigger();
	});

	// 단일 선택 체크박스 이벤트
	$('#agentStatusTable tbody').on('change', '.row-checkbox', function() {
		$('.row-checkbox').not(this).prop('checked', false); // 다른 체크박스 해제
		$('#agentStatusTable tbody tr').removeClass('selected'); // 모든 행에서 선택 효과 제거

		if ($(this).prop('checked')) {
			$(this).closest('tr').addClass('selected'); // 선택된 행에 배경색 적용
		}
	});

	// 데이터 테이블의 행 클릭 이벤트 추가
	$('#agentStatusTable tbody').off('click');
	$('#agentStatusTable tbody').on('click', 'td:not(:first-child)', async function() {
		const data = dataTable.row(this).data(); // 클릭한 행의 데이터 가져오기
		if (data) {
			await loadAgentStatusDetails(data); // 사용자 정보를 로드하는 함수 호출
		}
	});

}

// edittabWrap 탭 클릭 이벤트 추가
$(".edittabWrap .tabTitle li").on("click", function() {
	var editTabNum = $(".edittabWrap .tabTitle li").index($(this)); // 클릭하는 인덱스
	$(".edittabWrap .tabCont > li").removeClass("on"); // 기존 선택 해제
	$(".edittabWrap .tabCont > li:eq(" + editTabNum + ")").addClass("on"); // 클릭한 탭 활성화
	$(".edittabWrap .tabTitle li").removeClass("on"); // 모든 li의 테두리 제거
	$(this).addClass("on"); // 클릭한 탭 활성화
});

/* 등록 tab 활성화 */
$(".addtabWrap .tabTitle li").on("click", function(){
	var addTabNum = $(".addtabWrap .tabTitle li").index($(this));//click하는 인덱스
	$(".addtabWrap .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
	$(".addtabWrap .tabCont > li:eq(" + (addTabNum) + ")").addClass("on");
	$(".addtabWrap .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
	$(this).addClass("on");
});

/* 정책복사 tab 활성화 */
$(".copytabWrap .tabTitle li").on("click", function(){
	var copyTabNum = $(".copytabWrap .tabTitle li").index($(this));//click하는 인덱스
	$(".copytabWrap .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
	$(".copytabWrap .tabCont > li:eq(" + (copyTabNum) + ")").addClass("on");
	$(".copytabWrap .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
	$(this).addClass("on");
});

async function loadAgentStatusDetails(data) {
	cleanViewPopup();
	console.log("data:", data);
	
	// insertedBy가 null인 경우 공백으로 설정
	$("#agentStatusViewPopupInsertedBy").text(data.insertedBy || '');
	
	// insertDate가 null인 경우 공백으로 설정
	$("#agentStatusViewPopupInsertDate").text(data.insertDate ? new Date(data.insertDate).toLocaleString() : '');
	
	// updateBy가 null인 경우 공백으로 설정
	$("#agentStatusViewPopupUpdateBy").text(data.insertedBy || '');
	
	// updateDate가 null인 경우 공백으로 설정
	$("#agentStatusViewPopupUpdateDate").text(data.updateDate ? new Date(data.updateDate).toLocaleString() : '');
	
	$("#agentStatusViewPopupPolicyCode").val(data.agentPolicyCode);
	$("#agentStatusViewPopupPolicyName").val(data.policyName);
	
	const defaultPolicyText = data.defaultPolicyYn === 'Y' ? '기본' : '개별';
	// console.log("defaultPolicyText:", defaultPolicyText);
	$("#agentStatusViewPopupDefaultPolicyYn").text(defaultPolicyText);

	// 트레이메뉴 설정
	if(data.ztryMenuEnable === 1) {
		$("#ztryMenuEnable").prop("checked", true);
		toggleInputs("ztryMenuEnable", "zoneArea01");

		// 오픈영역 설정
		if(data.zopenArea === 1) {
			$("#zopenArea").prop("checked", true);
		}

		// 개인접근 설정
		if(data.zpersonalAccess === 1) {
			$("#zpersonalAccess").prop("checked", true);
		}

		// 옵션 표시 설정
		if(data.zoptionDisplay === 1) {
			$("#zoptionDisplay").prop("checked", true);
		}

		// SafetyZone 만들기 설정
		if(data.zsafetyZone === 1) {
			$("#zsafetyZone").prop("checked", true);
		}

		// 모든 영역 닫기 설정
		if(data.zareaClose === 1) {
			$("#zareaClose").prop("checked", true);
		}

		// 암호화 확인 설정	
		if(data.zenc === 1) {	
			$("#zenc").prop("checked", true);
		}
	}

	// 이동파일 만들기 체크박스
	$("#agentStatusViewPopupExportFileAuth").prop("checked", data.exportFileMenuEnable === 1);

	// 파일 전송 체크박스
	$("#agentStatusViewPopupFileTransfer").prop("checked", data.exportFileAuth === 1);

	// 사용자 콘솔 체크박스
	$("#agentStatusViewPopupWebManagerConsoleMenuEnable").prop("checked", data.webManagerConsoleMenuEnable === 1);
	$("#agentStatusViewPopupProductMenuEnable").prop("checked", data.productMenuEnable === 1);

	// 파일 전송존 로드
	loadFileZoneTable(data.policyId);
	
	// 팝업 내 탭 초기화
	$(".agentStatusViewPopup .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
	$(".agentStatusViewPopup .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
	$('.agentStatusViewPopup .tabTitle li:first-child').addClass("on"); //첫번째 li에 테두리를 추가해라
	$('.agentStatusViewPopup .tabCont > li:first-child').addClass("on"); //첫번째 탭을 선택해라


	// 수정 버튼 활성화
	$("#modifyBtn").removeClass("hide");
	// 등록 버튼 비활성화
	$("#saveBtn").addClass("hide");

	$(".agentStatusViewPopup").removeClass("hide");
}

function searchAgentStatus() {
	
	// val의 값으로 조건을 추가하고, 선택된 val의 텍스트를 필터 타이틀로 사용한다
	const searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
	const searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
	const searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
	const searchPolicyType = $(".searchAreaPopup .search_wrap select[name='searchPolicyType']");
	const searchCondition = $(".searchAreaPopup .search_wrap select[name='searchCondition']");
	const searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']");

	if(searchDateType.val() != null && searchDateType.val() != "") {
		if(!searchFieldValid('date', searchStartDate.val(), searchEndDate.val(), searchDateType.val())) {
			return;
		}
		var filterStr = searchStartDate.val() +' ~ '+ searchEndDate.val();
		addFilterItem(".agentStatus .searchBox", "기간", filterStr, 'searchAreaPopup');
	}
	else {
		resetSearchValue("기간");
	}

	if(searchPolicyType.val() != null && searchPolicyType.val() != "") {
		addFilterItem(".agentStatus .searchBox", "유형", searchPolicyType.find('option:selected').text(), 'searchAreaPopup');
	}
	else {
		resetSearchValue("유형");
	}

	if(searchCondition.val() != null && searchCondition.val() != "" && searchKeyword.val() != null && searchKeyword.val() != "") {
		if(!searchFieldValid('condition', searchKeyword.val(), null, null)) {
			return;
		}
		addFilterItem(".agentStatus .searchBox", "조건", searchCondition.find('option:selected').text() + " [" + searchKeyword.val() + "]", 'searchAreaPopup');
	}
	else {
		resetSearchValue("조건");
	}

	initializeDataTable();
	
    $('.searchAreaPopup').addClass('hide');
}

function removeFilter(element, popupName) {

    if(popupName == 'searchAreaPopup') {
        // 선택된 필터의 filterTitle을 찾아서 초기화함수 호출
        var filterTitle = $(element).closest('.filterItem').find('.filterTitle').text();
        resetSearchValue(filterTitle);

        $(element).closest('.filterItem').remove(); // 해당 필터 아이템 제거
    }
    else {
        console.log('removeFilter: no match popupName: ', popupName);
    }
    
    initializeDataTable();
}

function resetSearchValue(filterTitle) {
    console.log('resetSearchValue: ', filterTitle);

	var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
	var searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
	var searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
	var searchPolicyType = $(".searchAreaPopup .search_wrap select[name='searchPolicyType']");
	var searchCondition = $(".searchAreaPopup .search_wrap select[name='searchCondition']");
	var searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']");
    
    if(filterTitle == "기간") {
        searchDateType.val(searchDateType.find('option[value=""]').val());
        searchStartDate.val('');
        searchEndDate.val('');
    }   
    else if(filterTitle == "유형") {
        searchPolicyType.val(searchPolicyType.find('option[value=""]').val());
    }
    else if(filterTitle == "조건") {
        searchCondition.val(searchCondition.find('option[value="all"]').val());
        searchKeyword.val('');
    }
    
}

// 각 체크박스를 토글하는 함수
function toggleInputs(zoneId, areaId) {
    // zoneId 체크박스가 체크되었을 때, 관련 areaId의 체크박스들의 disabled 속성을 토글
    const zoneCheckbox = document.getElementById(zoneId); // zone 체크박스
    const areaInputs = document.querySelectorAll(`#${areaId} input[type="checkbox"]`); // 관련된 area 체크박스들

    // zone 체크박스 상태에 따라 area 체크박스의 disabled 속성을 설정
    areaInputs.forEach(input => {
        input.disabled = !zoneCheckbox.checked; // zone 체크박스가 체크되지 않으면 비활성화
    });
}

// 각 체크박스의 상태에 따라 관련 input의 disabled 속성을 설정
document.getElementById("ztryMenuEnable").addEventListener("change", function() {
    toggleInputs("ztryMenuEnable", "zoneArea01"); // ztryMenuEnable 체크박스를 토글하면 zoneArea01의 input들이 토글됨
});

// document.getElementById("ori01").addEventListener("change", function() {
//     toggleInputs("ori01", "oriArea01"); // ori01 체크박스를 토글하면 oriArea01의 input들이 토글됨
// });


// $(document).ready(function() {
//     $("#fileZoneTableCheckbox").on("change", function() {
//         if(this.checked) {
//             $("#fileZoneTableWrap").removeClass("hide");
//         } else {
//             $("#fileZoneTableWrap").addClass("hide");
//         }
//     });
// });


function cleanViewPopup() {
	// 팝업 초기화
	$("#agentStatusViewPopupInsertedBy").text("");
	$("#agentStatusViewPopupInsertDate").text("");
	$("#agentStatusViewPopupUpdateBy").text("");
	$("#agentStatusViewPopupUpdateDate").text("");

	$("#agentStatusViewPopupPolicyCode").val("");
	$("#agentStatusViewPopupPolicyName").val("");
	$("#agentStatusViewPopupDefaultPolicyYn").text("");

	// zoneArea01 체크박스 초기화
	$("#ztryMenuEnable").prop("checked", false);
	toggleInputs("ztryMenuEnable", "zoneArea01");
	$("#zopenArea").prop("checked", false);
	$("#zpersonalAccess").prop("checked", false);
	$("#zoptionDisplay").prop("checked", false);
	$("#zsafetyZone").prop("checked", false);
	$("#zareaClose").prop("checked", false);
	$("#zenc").prop("checked", false);

	// oriArea01 체크박스 초기화
	// $("#agentStatusViewPopupOpenORIZONFileExplorer").prop("checked", false);
	// $("#agentStatusViewPopupEncryptionCheckAgain").prop("checked", false);

	// fileTransfer 체크박스 초기화
	$("#agentStatusViewPopupFileTransfer").prop("checked", false);
	$("#agentStatusViewPopupExportFileAuth").prop("checked", false);
	$("#agentStatusViewPopupWebManagerConsoleMenuEnable").prop("checked", false);

	// fileZoneTableWrap 초기화
	$("#fileZoneTableCheckbox").prop("checked", false);
	//$("#fileZoneTableWrap").addClass("hide");

	$("#policyInfo").removeClass("hide");

	// productMenu 체크박스 초기화
	$("#agentStatusViewPopupProductMenuEnable").prop("checked", false);
}

async function openAddPopup() {

	cleanViewPopup();

	var data = await loadMyInfo();
	console.log("data:", data);

	// 파일 전송존 테이블 로드
	loadFileZoneTable(0);
	
	// 등록자
	$("#agentStatusViewPopupInsertedBy").text(data.userName);

	// 수정자
	$("#agentStatusViewPopupUpdateBy").text(data.userName);
	
	// policyInfo 숨기기
	$("#policyInfo").addClass("hide");

	// 정책 유형
	$("#agentStatusViewPopupDefaultPolicyYn").text("개별");

	// 등록 버튼 활성화
	$("#saveBtn").removeClass("hide");
	// 수정 버튼 비활성화
	$("#modifyBtn").addClass("hide");


	$(".agentStatusViewPopup").removeClass("hide");
}


function loadFileZoneTable(policyId) {

	var count = 0;
    // 전체 FileZone 목록을 먼저 로드
    fileZoneTable = initDataTables('#fileZoneTable', {
        ajax: {
            url: '/primx/fun/sys/fileZone/list.do',
            type: 'GET'
        },
        paging: false,
        lengthChange: false,
        info: false,
        columnDefs: [
            {
                targets: 0,
                className: 'result'    
            }
        ],
        columns: [
            { data: null },
            { data: 'proto' },
            { data: 'zoneNm' }
        ],
        columnDefs: [
            {
                render: DataTable.render.select(),
                targets: 0
            }
        ],
        select: {
            style: 'multi',
            selector: 'td:first-child'
        },
        // 테이블 초기 로드가 완료된 후
        initComplete: function() {
            if (policyId > 0) {
                // 매핑된 zone 정보 로드
                $.ajax({
                    url: '/primx/fun/sys/fileZone/selectFileZone.do',
                    type: 'GET',
                    data: { policyId: policyId },
                    success: function(response) {
                        if (response.data) {
                            // 매핑된 각 zone에 대해
                            response.data.forEach(function(mappedZone) {
                                // fileZoneTable의 모든 row를 순회
                                fileZoneTable.rows().every(function(rowIdx) {
                                    var rowData = this.data();
                                    // zoneId가 일치하는 row를 찾아서 선택
                                    if (rowData.zoneId === mappedZone.zoneId) {
                                        fileZoneTable.row(rowIdx).select();
                                        count++;
                                    }
                                });
                            });
                        }

						if(count > 0) {
							$("#fileZoneTableWrap").removeClass("hide");
							$("#fileZoneTableCheckbox").prop("checked", true);
						}
                    },
                    error: function(xhr, status, error) {
                        console.error("매핑된 FileZone 조회 실패:", error);
                    }
                });
            }
        }
    });
}

function saveAgentStatus() {

	// 정책 명 가져오기
	var policyName = $("#agentStatusViewPopupPolicyName").val();

	// 트레이메뉴 설정
	var ztryMenuEnable = $("#ztryMenuEnable").prop("checked") ? 1 : 0;

	// 오픈영역 설정
	var zopenArea = $("#zopenArea").prop("checked") ? 1 : 0;

	// 개인접근 설정
	var zpersonalAccess = $("#zpersonalAccess").prop("checked") ? 1 : 0;

	// 옵션 표시 설정
	var zoptionDisplay = $("#zoptionDisplay").prop("checked") ? 1 : 0;

	// SafetyZone 만들기 설정
	var zsafetyZone = $("#zsafetyZone").prop("checked") ? 1 : 0;

	// 모든 영역 닫기 설정
	var zareaClose = $("#zareaClose").prop("checked") ? 1 : 0;

	// 암호화 확인 설정
	var zenc = $("#zenc").prop("checked") ? 1 : 0;

	// 체크박스 체크되어있으면 1, 아니면 0
	// 이동파일 만들기 체크박스 가져오기
	var exportFileAuth = $("#agentStatusViewPopupExportFileAuth").prop("checked") ? 1 : 0;

	// 파일 전송 체크박스 가져오기
	var fileTransfer = $("#agentStatusViewPopupFileTransfer").prop("checked") ? 1 : 0;

	// 사용자 콘솔 체크박스 가져오기
	var webManagerConsoleMenuEnable = $("#agentStatusViewPopupWebManagerConsoleMenuEnable").prop("checked") ? 1 : 0;

	// 파일전송 Zone 체크박스 가져오기
	var fileZoneTableCheckbox = $("#fileZoneTableCheckbox").prop("checked") ? 1 : 0;

	// PRIMXCS Agent정보 체크박스 가져오기
	var productMenuEnable = $("#agentStatusViewPopupProductMenuEnable").prop("checked") ? 1 : 0;


	// 정책명이 비어있으면 경고창 띄우기
	if(policyName == "") {
		alert("정책명을 입력해주세요.");
		return;
	}

	// fileZoneTable에 체크된 존 정보 가져오기
	var checkedZones = fileZoneTable.rows({
		selected: true
	}).data();

	console.log("checkedZones:", checkedZones);

	// 전달인자 생성
	var agentPolicyMasterVO = {
		policyName: policyName,
		defaultPolicyYn: 'N',
		exportFileAuth: fileTransfer,
		webManagerAuth: fileTransfer,
		notEncryptionPathAuth: 0,
		productMenuEnable: productMenuEnable,
		webManagerConsoleMenuEnable: webManagerConsoleMenuEnable,
		exportFileMenuEnable: exportFileAuth,
		ztryMenuEnable: ztryMenuEnable,
		zopenArea: zopenArea,
		zpersonalAccess: zpersonalAccess,
		zoptionDisplay: zoptionDisplay,
		zsafetyZone: zsafetyZone,
		zareaClose: zareaClose,
		zenc: zenc
	};

	// 파일전송 Zone 체크박스 체크되어 있으면 신규 리스트 전달인자 생성
	var fileZoneTableData = [];
	if(fileZoneTableCheckbox == 1) {
		checkedZones.each(function(zone) {
			fileZoneTableData.push({
				zoneId: zone.zoneId,
				zoneNm: zone.zoneNm,
				proto: zone.proto
			});
		});
	}

	console.log("agentPolicyMasterVO:", agentPolicyMasterVO);
	console.log("fileZoneTableData:", fileZoneTableData);

	// ajax로 전달시 agentPolicyMasterVO 객체 하나, fileZoneTableData 리스트 하나 전달
	$.ajax({
		url: '/primx/fun/pol/agent/policy/save.do',
		type: 'POST',
        contentType: 'application/json',  // JSON 형식으로 전송
		data: JSON.stringify({
			agentPolicyMasterVO: agentPolicyMasterVO,
			fileZoneTableData: fileZoneTableData
		}),
		success: function(response) {
			if(response.status == "success") {
				alert("저장되었습니다.");
				initializeDataTable();
				$(".agentStatusViewPopup").addClass("hide");
			} else {
				alert("저장에 실패했습니다.");
			}
		},
		error: function(error) {
			console.log("error:", error);
		}
	});
}

function updateAgentStatus() {

	// 정책 ID
	var agentPolicyCode = $("#agentStatusViewPopupPolicyCode").val();

	// 정책 명 가져오기
	var policyName = $("#agentStatusViewPopupPolicyName").val();

	// 트레이메뉴 설정
	var ztryMenuEnable = $("#ztryMenuEnable").prop("checked") ? 1 : 0;

	// 오픈영역 설정
	var zopenArea = $("#zopenArea").prop("checked") ? 1 : 0;

	// 개인접근 설정
	var zpersonalAccess = $("#zpersonalAccess").prop("checked") ? 1 : 0;

	// 옵션 표시 설정
	var zoptionDisplay = $("#zoptionDisplay").prop("checked") ? 1 : 0;
	
	// SafetyZone 만들기 설정
	var zsafetyZone = $("#zsafetyZone").prop("checked") ? 1 : 0;

	// 모든 영역 닫기 설정
	var zareaClose = $("#zareaClose").prop("checked") ? 1 : 0;

	// 암호화 확인 설정
	var zenc = $("#zenc").prop("checked") ? 1 : 0;

	// 체크박스 체크되어있으면 1, 아니면 0
	// 이동파일 만들기 체크박스 가져오기
	var exportFileAuth = $("#agentStatusViewPopupExportFileAuth").prop("checked") ? 1 : 0;

	// 파일 전송 체크박스 가져오기
	var fileTransfer = $("#agentStatusViewPopupFileTransfer").prop("checked") ? 1 : 0;

	// 사용자 콘솔 체크박스 가져오기
	var webManagerConsoleMenuEnable = $("#agentStatusViewPopupWebManagerConsoleMenuEnable").prop("checked") ? 1 : 0;

	// 파일전송 Zone 체크박스 가져오기
	var fileZoneTableCheckbox = $("#fileZoneTableCheckbox").prop("checked") ? 1 : 0;

	// PRIMXCS Agent정보 체크박스 가져오기
	var productMenuEnable = $("#agentStatusViewPopupProductMenuEnable").prop("checked") ? 1 : 0;

	// 기본 정책 유무
	var defaultPolicyYn = $("#agentStatusViewPopupDefaultPolicyYn").text() == '기본' ? 'Y' : 'N';

	// 정책명이 비어있으면 경고창 띄우기
	if(policyName == "") {
		alert("정책명을 입력해주세요.");
		return;
	}

	// fileZoneTable에 체크된 존 정보 가져오기
	var checkedZones = fileZoneTable.rows({
		selected: true
	}).data();

	console.log("checkedZones:", checkedZones);

	// 전달인자 생성
	var agentPolicyMasterVO = {
		policyName: policyName,
		agentPolicyCode: agentPolicyCode,
		defaultPolicyYn: defaultPolicyYn,
		exportFileAuth: fileTransfer,
		webManagerAuth: fileTransfer,
		notEncryptionPathAuth: 0,
		productMenuEnable: productMenuEnable,
		webManagerConsoleMenuEnable: webManagerConsoleMenuEnable,
		exportFileMenuEnable: exportFileAuth,
		ztryMenuEnable: ztryMenuEnable,
		zopenArea: zopenArea,
		zpersonalAccess: zpersonalAccess,
		zoptionDisplay: zoptionDisplay,
		zsafetyZone: zsafetyZone,
		zareaClose: zareaClose,
		zenc: zenc
	};

	// 파일전송 Zone 체크박스 체크되어 있으면 신규 리스트 전달인자 생성
	var fileZoneTableData = [];
	if(fileZoneTableCheckbox == 1) {
		checkedZones.each(function(zone) {
			fileZoneTableData.push({
				zoneId: zone.zoneId,
				zoneNm: zone.zoneNm,
				proto: zone.proto
			});
		});
	}

	console.log("agentPolicyMasterVO:", agentPolicyMasterVO);
	console.log("fileZoneTableData:", fileZoneTableData);

	// ajax로 전달시 agentPolicyMasterVO 객체 하나, fileZoneTableData 리스트 하나 전달
	$.ajax({
		url: '/primx/fun/pol/agent/policy/update.do',
		type: 'POST',
        contentType: 'application/json',  // JSON 형식으로 전송
		data: JSON.stringify({
			agentPolicyMasterVO: agentPolicyMasterVO,
			fileZoneTableData: fileZoneTableData
		}),
		success: function(response) {
			if(response.status == "success") {
				alert("저장되었습니다.");
				initializeDataTable();
				$(".agentStatusViewPopup").addClass("hide");
			} else {
				alert("저장에 실패했습니다.");
			}
		},
		error: function(error) {
			console.log("error:", error);
		}
	});
}

async function copyAgentPolicy() {
    // agentStatusTable 테이블의 선택된 row 가져오기
    var selectedCheckbox = $("#agentStatusTable tbody input[type='checkbox']:checked");

    // 선택된 row가 0,1,n개인지 확인
    if(selectedCheckbox.length == 0 || selectedCheckbox.length > 1){
        alert('복사할 정책을 하나 선택해주세요.');
        return;
    }

    cleanViewPopup();

    // 선택된 행의 데이터 가져오기
    const selectedRow = selectedCheckbox.closest('tr');
    const data = dataTable.row(selectedRow).data();
    
    console.log("selected data:", data);
    
    if (data) {
        await loadAgentStatusDetails(data);
        
        // 선택한 row의 정책명 + _copy 붙여서 정책명 설정
        $("#agentStatusViewPopupPolicyName").val(data.policyName + "_copy");
		$("#agentStatusViewPopupPolicyCode").val("");

		$("#policyInfo").addClass("hide");
        
        // 등록 버튼 활성화
        $("#saveBtn").removeClass("hide");
        // 수정 버튼 비활성화
        $("#modifyBtn").addClass("hide");
    }
    
    $(".agentStatusViewPopup").removeClass("hide");
}

function deleteAgentPolicy() {
    // agentStatusTable 테이블의 선택된 row 가져오기
    var selectedCheckbox = $("#agentStatusTable tbody input[type='checkbox']:checked");

    // 선택된 row가 0,1,n개인지 확인
    if(selectedCheckbox.length == 0 || selectedCheckbox.length > 1){
        alert('삭제할 정책을 하나 선택해주세요.');
        return;
    }

    $(".agnDelPopup").removeClass("hide");
}

function approveDeleteAgentPolicy() {
	// agentStatusTable 테이블의 선택된 row 가져오기
	var selectedCheckbox = $("#agentStatusTable tbody input[type='checkbox']:checked");

	// 선택된 row가 0,1,n개인지 확인
	if(selectedCheckbox.length == 0 || selectedCheckbox.length > 1){
		alert('삭제할 정책을 하나 선택해주세요.');
		return;
	}

	// 선택된 행의 데이터 가져오기
	const selectedRow = selectedCheckbox.closest('tr');
	const data = dataTable.row(selectedRow).data();

	console.log("selected data:", data);

	// agentPolicyCode
	var policyId = data.policyId;
	var agentPolicyCode = data.agentPolicyCode;
	console.log("policyId:", policyId);
	console.log("agentPolicyCode:", agentPolicyCode);

	$.ajax({
		url: '/primx/fun/pol/agent/policy/delete.do',
		type: 'POST',
		data: {
			policyId: policyId,
			agentPolicyCode: agentPolicyCode
		},
		success: function(response) {
			if(response.status == "success") {
				alert("삭제되었습니다.");
				initializeDataTable();
				$(".agnDelPopup").addClass("hide");
			} else {
				alert("삭제에 실패했습니다.");
			}
		},
		error: function(error) {
			console.log("error:", error);
		}
	});
	
}

function initDefaultAgentPolicyChange() {
	
    // agentStatusTable 테이블의 선택된 row 가져오기
    var selectedRow = $("#agentStatusTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });

    // 선택된 row가 0,1,n개인지 확인
    if(selectedRow.length == 0 || selectedRow.length > 1){
        alert('기본 정책 적용할 정책을 하나 선택해주세요.');
        return;
    }

    $(".defaultChangePopup").removeClass('hide');
}

function applyDefaultAgentPolicy() {

    // agentStatusTable 테이블의 선택된 row 가져오기
    var selectedRow = $("#agentStatusTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });

    // 선택된 row의 정보 모두 가져오기
    var selectedPolicyInfo = selectedRow.find('td');
    console.log('selectedPolicyInfo: ', selectedPolicyInfo);

    // 정책 ID 가져오기
    var agentPolicyCode = selectedPolicyInfo.eq(3).text();
    console.log('agentPolicyCode: ', agentPolicyCode);

    $.ajax({
        url: '/primx/fun/pol/agent/policy/applyAgentDefaultPolicy.do',
        type: 'POST',
        data: {
            agentPolicyCode: agentPolicyCode
        },
        success: function(data){
            console.log('data: ', data);
            if(data.status === "success"){
                alert('기본 정책 적용 성공');
            } else {
                alert('기본 정책 적용 실패: ' + data.message);
            }
            $(".defaultChangePopup").addClass('hide');
            initializeDataTable();
        },
        error: function(xhr, status, error){
            console.error('error: ', error);
        }
    });
    $(".defaultChangePopup").addClass('hide');

}


function openSystemStatusPopup(){

    // polSysStatusTable 테이블의 선택된 row 가져오기
    var selectedRow = $("#agentStatusTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });

    // 선택된 row가 0,1,n개인지 확인
    if(selectedRow.length == 0 || selectedRow.length > 1){
        alert('시스템 현황을 볼 정책을 하나 선택해주세요.');
        return;
    }
    // 선택된 row의 정보 모두 가져오기
    var selectedPolicyInfo = selectedRow.find('td');

    // 정책 ID 가져오기
    var agentPolicyCode = selectedPolicyInfo.eq(3).text();
    console.log('agentPolicyCode: ', agentPolicyCode);

    // polSysStatusPopupHeaderName 에 정책명 policyCode로 변경
    $('#polSysStatusPopupHeaderName').text(agentPolicyCode+' > ');

    // 정책 정보 가져오기
    loadSysStatusTable(agentPolicyCode);

    
    $('.agnSysStatusPopup').removeClass('hide');
}

function searchSystem(){
    loadSysStatusTable($('#polSysStatusPopupAgentPolicyCode').val());
}

function loadSysStatusTable(agentPolicyCode){

	// 기존 테이블이 있다면 제거
	if (polSysStatusTable) {
		polSysStatusTable.destroy();
		polSysStatusTable = null;
	}

	// 기존 CSV 버튼 이벤트 바인딩 제거
	$('#agnSysStatusSysCsvExport').off('click');
	
    $('#polSysStatusPopupAgentPolicyCode').val(agentPolicyCode);
	polSysStatusTable = initDataTables('#polSysStatusTable' ,{
		ajax: {
			url: '/primx/fun/sys/systemStatus/list.do',
			type: 'POST',
			data: function(data) {
				data.agentPolicyCode = agentPolicyCode;
				data.searchType = 'computer'
				data.searchWord = $('#searchWord').val();
			},
			dataSrc: function (json) {
                console.log('json: ', json);
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
	        /*{
				data: null,
				render: function (data, type, row, meta) {
					return getPolicyZone(row, 'OZ');
				}
			},*/
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
	        { data: 'userNm' },
	        { data: 'deptName' }
	        /*{ data: 'winVersionString' },
	        { data: 'agentVersion' }*/
	        // { data: 'policyName' },
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
		}
	});
		
	// CSV 내보내기 버튼과 연결
	$('#agnSysStatusSysCsvExport').on('click', function() {
		polSysStatusTable.button('.buttons-csv').trigger();
	});
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

function initDeleteSystem(){
    $('.agnSysDelPopup').removeClass('hide');
}

function deleteSystem(){
    
    // agnSysStatusAddTable 테이블의 선택된 row 가져오기
    var selectedRow = $("#polSysStatusTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });

    // 선택된 row가 0,1,n개인지 확인
    if(selectedRow.length == 0){
        alert('시스템 제거할 시스템을 하나 이상 선택해주세요.');
        return;
    }

    // 선택된 row의 정보 모두 가져오기
    var selectedSystemInfoList = [];
    selectedRow.each(function() {
        var selectedSystemInfo = $(this).find('td');
        selectedSystemInfoList.push(selectedSystemInfo);
    });
    console.log('selectedSystemInfoList: ', selectedSystemInfoList);

    // 시스템 ID 가져오기
    var agentIdList = [];
    selectedSystemInfoList.forEach(function(systemInfo) {
        var agentId = $(systemInfo[2]).text();
        agentIdList.push(agentId);
    });
    console.log('agentIdList: ', agentIdList);
    
    $.ajax({
        url: '/primx/fun/pol/agent/policy/deleteSystem.do',
        type: 'POST',
		traditional: true,
        data: {
            agentIdList: agentIdList
        },
        success: function(data){
            console.log('data: ', data);
            if(data.status === "success"){
                alert('시스템 제거 성공');
            } else {
                alert('시스템 제거 실패: ' + data.message);
            }
            $('.agnSysDelPopup').addClass('hide');
            loadSysStatusTable($('#polSysStatusPopupAgentPolicyCode').val());
			initializeDataTable();
        },
        error: function(xhr, status, error){
            console.error('error: ', error);
        }
    });

    $('.agnSysDelPopup').addClass('hide');
}

function initAddSystem(){

    // 정책 정보 가져오기
    loadAddSysStatusTable(null);

    $('.agnSysStatusAddPopup').removeClass('hide');
}

function searchAddSystem(){
    loadAddSysStatusTable(null);
}

function loadAddSysStatusTable(agentPolicyCode){

	agnSysStatusAddTable = initDataTables('#agnSysStatusAddTable ' ,{
		ajax: {
			url: '/primx/fun/sys/systemStatus/list.do',
			type: 'POST',
			data: function(data) {
				data.agentPolicyCode = agentPolicyCode;
				data.searchType = 'computer';
				data.searchWord = $('#searchWordAddSystem').val();
			},
			dataSrc: function (json) {
                console.log('json: ', json);
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
	        /*{
				data: null,
				render: function (data, type, row, meta) {
					return getPolicyZone(row, 'OZ');
				}
			},*/
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
	        { data: 'userNm' },
	        { data: 'deptName' }
	        /*{ data: 'winVersionString' },
	        { data: 'agentVersion' }*/
	        // { data: 'policyName' },
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
		select: {
			style: 'multi',
			selector: 'td:first-child'
		}
	});
    
    // draw 이벤트 핸들러 추가
	agnSysStatusAddTable.on('draw', function () {
        var currentAgentPolicyCode = $('#polSysStatusPopupAgentPolicyCode').val(); // 현재 정책 코드
        
        // 테이블의 모든 행을 순회
        agnSysStatusAddTable.rows().every(function(rowIdx) {
            var rowData = this.data();
            var row = $(this.node()); // 현재 행의 DOM 요소
            
            // rowData의 policyCode가 현재 정책 코드와 같으면 disTr 클래스 추가
            if (rowData && rowData.agentPolicyCode === currentAgentPolicyCode) {
                row.addClass('disTr');
            }
        });
    });
}

function addSystem(){
    // 선택된 시스템 정보 가져오기
    var selectedRow = $("#agnSysStatusAddTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });
    // 여러개 선택되어 있는지 확인
    if(selectedRow.length == 0){
        alert('시스템 추가할 시스템을 하나 이상 선택해주세요.');
        return;
    }

    // 선택된 시스템 정보 가져오기
    var selectedSystemInfoList = [];
    selectedRow.each(function() {
        var selectedSystemInfo = $(this).find('td');
        selectedSystemInfoList.push(selectedSystemInfo);
    });
    console.log('selectedSystemInfoList: ', selectedSystemInfoList);

    // 시스템 ID 가져오기
    var agentIdList = [];
    selectedSystemInfoList.forEach(function(systemInfo) {
        var agentId = $(systemInfo[2]).text();
        agentIdList.push(agentId);
    });
    console.log('agentIdList: ', agentIdList);

    // 정책 코드 가져오기
    var agentPolicyCode = $('#polSysStatusPopupAgentPolicyCode').val();
    console.log('agentPolicyCode: ', agentPolicyCode);
    
    $.ajax({
        url: '/primx/fun/pol/agent/policy/addSystem.do',
        type: 'POST',
		traditional: true,
        data: {
            agentIdList: agentIdList,
            agentPolicyCode: agentPolicyCode
        },
        success: function(data){
            console.log('data: ', data);
            if(data.status === "success"){
                alert('시스템 추가 성공');
                loadSysStatusTable($('#polSysStatusPopupAgentPolicyCode').val());
                initializeDataTable();
                $('.agnSysStatusAddPopup').addClass('hide');
            } else {
                alert('시스템 추가 실패: ' + data.message);
            }
        },
        error: function(xhr, status, error){
            console.error('error: ', error);
        }
    });
}
