/* 설정 > 부서 및 사용자 설정 */
let userTable = null;
let selectedDeptId = null;
let selectedDeptName = null;
let selectedcompanyId = null;
let deptTree = null;
let isUserIdValid = false; // 사용자 ID 유효성 플래그
let isDeptIdValid = false; // 사용자 ID 유효성 플래그

let distributionTable = null;

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

	/* 데이터 테이블 */	
	loadDistributionTable();
	/* 부서 트리 로드 */
	loadDeptTree();
	
	/* datepicker */
	datePicker();
}

function initDistributionAddBtn() {
	clearDistributionAddBtn();


	if(selectedDeptId == null) {
		alert('부서를 선택해주세요.');
		return;
	}

	$("#deptId").val(selectedDeptId);
	$("#deptName").val(selectedDeptName);

	$(".distributionPolicyPopup").removeClass("hide");
}

function clearDistributionAddBtn() {
	$("#deptId").val('');
	$("#deptName").val('');
	$("#SetDeploymentPeriod").val('1');
	$("#startDt4").val('');
	$("#endDt4").val('');
	$("#releaseYn").prop('checked', false);
	$("#includeSubDept").prop('checked', false);
	$(".datepicker").show().attr('disabled', false);
	$(".distributionPolicyPopup").addClass("hide");
}

function loadDistributionTable(){
	// 기존 테이블이 있다면 제거
	if (distributionTable) {
		distributionTable.destroy();
		distributionTable = null;
	}

	// 기존 CSV 버튼 이벤트 바인딩 제거	
	$('#distributionPolicyCsvExport').off('click');
	distributionTable = initDataTables('#distributionTable', { 
		
		ajax: {
			url: '/primx/fun/pol/distribution/list.do',
			type: 'POST',
			data: function(d) {
				// 검색의 조건을 넣어준다.
				d.searchDeploymentStatus = $("#searchDeploymentStatus").val();
				d.searchDateType = $("#searchDateType").val();
				d.searchStartDate = $("#startDt3").val();
				d.searchEndDate = $("#endDt3").val();
				d.searchDeptId = $("#searchDeptId").val();
				d.searchDeptName = $("#deptNameSearch").val();
				d.searchProductName = "AGENT";
				console.log('data: ', d);
			},
			dataSrc: 'resultList',
		},
		processing: true,
		serverSide: true,
		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: {
				pageLength: {
					menu: [ 10, 25, 50, 100 ]
				}
			},
			bottomEnd: {
				info : true,
				paging: {
					buttons: 5
				},
			},
		},
		searching: false,
		ordering: false,
	    language :{
			emptyTable: "No Data.",
		},
		columns: [
			// 배포 상태: 중지, 배포 (중지: 종료일이 현재일 이전, 배포: 종료일이 현재일 이후)
			{ data: null, render: function(data, type, row, meta) {
				return isdeployment(row.releaseYn, row.endDate);
			},
			createdCell: function (td, cellData, rowData, row, col) {
				// td에 클래스 추가
				$(td).addClass('result');
			} },
			// 부서 명
			{ data: 'deptName' },
			// 부서 경로
			{ data: 'deptPath'}, 
			// 시작일
			{ data: 'beginDate', render: function(data) {
				return new Date(data).toISOString().split('T')[0];
			} },
			// 종료일
			{ data: 'endDate', render: function(data) {
				return new Date(data).toISOString().split('T')[0];
			} },
			// 컴퓨터 수
			{ data: 'computerCnt' },
			// 최신 업데이트 개수
			{ data: 'latestUpdateCnt' }
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
					columns: ':visible'  // 보이는 열만 내보내기
				},
				bom: true, // UTF-8 BOM 추가
				customize: function(csv) {
					return '\ufeff' + csv; // UTF-8 BOM 추가하여 한글 깨짐 방지
				}
			}
		],
		select: {
			style: 'single',
			selector: 'td:first-child'
		}
	});


	$('#distributionTable tbody').on('click', 'td:not(:first-child)', async function() {
		const data = distributionTable.row(this).data(); // 클릭한 행의 데이터 가져오기
		if (data) {
			loadDistributionDetails(data); // 사용자 정보를 로드하는 함수 호출
		}
	});

	// CSV 내보내기 버튼과 연결
    $('#distributionPolicyCsvExport').on('click', function() {
        distributionTable.button('.buttons-csv').trigger();
    });
}

// 중지 배포 설절해주는 함수 호출
function isdeployment(releaseYn, endDate) {
	// releaseYn == Y 이고 endDate가 오늘 보다 큰 경우 배포
	if(releaseYn == 'Y' && new Date(endDate) > new Date()) {
		return '<td class="result"><span class="approval">배포</span></td>';
	}
	// 위 경우가 아닌 경우 중지
	else {
		return '<td class="result"><span class="return">중지</span></td>';
	}
}

function loadDistributionDetails(data) {
	clearDistributionAddBtn();
	
	$("#deptId").val(data.deptId);
	$("#deptName").val(data.deptName);

	// releaseYn == Y 이고 endDate가 오늘 보다 큰 경우 배포
	if(data.releaseYn == 'Y' && new Date(data.endDate) > new Date()) {
		$("#releaseYn").prop('checked', true);
	}
	// releaseYn == N 이고 endDate가 오늘 보다 작은 경우 중지
	else {
		$("#releaseYn").prop('checked', false);
	}

	// 날짜를 YYYY-MM-DD 형식으로 변환
	const beginDate = new Date(data.beginDate).toISOString().split('T')[0];
	const endDate = new Date(data.endDate).toISOString().split('T')[0];
	
	// 종료일이 9999-12-31인 경우 무기한으로 설정
	if (endDate === '9999-12-30') {
		$("#SetDeploymentPeriod").val('0');
		$("#startDt4").val('').hide().attr('disabled', true);
		$("#endDt4").val('9999-12-30').attr('disabled', true);
	} else {
		$("#SetDeploymentPeriod").val('1');
		$("#startDt4").val(beginDate).show().attr('disabled', false);
		$("#endDt4").val(endDate).attr('disabled', false);
	}

	$(".distributionPolicyPopup").removeClass("hide");
}

function clearSearchPopup() {
	$('#searchDeploymentStatus').val('');
	$('#searchDateType').val('');
	$('#startDt3').val('');
	$('#endDt3').val('');
	$('#searchDeptId').val('');
	$('#deptNameSearch').val('');
}

function initSearchPopup() {
	clearSearchPopup();
	$('.distuserSearchPopup').removeClass('hide');
	$('.distuserSearchPopup #deptNameSearch').focus();
}

function handleUserSearch() {
	loadDistributionTable();
	$('.distuserSearchPopup').addClass('hide');
}

// updateDeptTree 함수를 먼저 정의
function updateDeptTree(treeData, isSearchMode) {
	const container = document.querySelector('.departTreeMap');
	if (!container) return;

	const tree = new CustomTree(container, {
		searchMode: isSearchMode,
		expandAll: true,
		
		onSelect: function(deptName, deptId, companyId) {
			// 클릭 시에 선택 상태 저장
			selectedDeptId = deptId;
			selectedDeptName = deptName;
			console.log('부서 선택됨:', {
				selectedDeptId: selectedDeptId,
				selectedDeptName: selectedDeptName,
				companyId: companyId
			});
			clearSearchPopup();
			$("#searchDeptId").val(deptId);
			loadDistributionTable();
		}
	});

	tree.render(treeData);
	deptTree = tree;
}

function loadDeptTree(searchKeyword) {
	return new Promise((resolve, reject) => {
		$.ajax({
			url: '/primx/fun/dpt/tree.do',
			type: 'POST',
			data: {
				searchKeyword: searchKeyword || ''
			},
			success: function(response) {
				if (response && response.data) {
					updateDeptTree(response.data, !!searchKeyword);
					resolve();
				} else {
					reject('트리 데이터가 없습니다.');
				}
			},
			error: function(xhr, status, error) {
				console.error("부서 트리 로드 실패:", error);
				reject(error);
			}
		});
	});
}

function initDeploymentEventListeners() {
    // datepicker 초기화
    $('.datepicker').datepicker({
        dateFormat: 'yy-mm-dd',
        onSelect: function (dateText, inst) {
            $(this).datepicker('hide');
        }
    });

    // 사용기간 select box 변경 이벤트 처리
    $('#SetDeploymentPeriod').on('change', function () {
        var selectedValue = $(this).val();
        var $startDateInput = $('#startDt4');
        var $endDateInput = $('#endDt4');

        if (selectedValue === '0') { // 무기한
            $startDateInput.hide().attr('disabled', true);
            $endDateInput
                .val('9999-12-31')
                .attr('disabled', true);
        } else { // 기간 지정
            $startDateInput.show().attr('disabled', false);
            $endDateInput
                .val('')
                .attr('disabled', false);
        }
    });

    // 페이지 로드 시 초기 상태 설정
    $('#SetDeploymentPeriod').trigger('change');
}

// 이벤트 리스너 초기화 실행
$(document).ready(function () {
    initDeploymentEventListeners();
});

function handleDistributionSave() {

	const deptId = $("#deptId").val();
	const beginDate = $("#startDt4").val();
	var endDate = $("#endDt4").val();
	const releaseYn = $("#releaseYn").prop('checked') ? 'Y' : 'N';
	const includeSubDept = $("#includeSubDept").prop('checked') ? 'Y' : 'N';

	const SetDeploymentPeriod = $("#SetDeploymentPeriod").val();


	if(SetDeploymentPeriod == '0') {
		endDate = '9999-12-31';
	}
	else {
		if(beginDate == '') {
			alert('시작일을 입력해주세요.');
			return;
		}
		// 시작일이 오늘 보다 작을 수 없음
		if(new Date(beginDate) < new Date()) {
			alert('시작일은 오늘 보다 작을 수 없습니다.');
			return;
		}
		if(endDate == '') {
			alert('종료일을 입력해주세요.');
			return;
		}
		// 종료일이 오늘 보다 작을 수 없음
		if(new Date(endDate) < new Date()) {
			alert('종료일은 오늘 보다 작을 수 없습니다.');
			return;
		}
		// 종료일은 시작일보다 작을 수 없음
		if(new Date(endDate) < new Date(beginDate)) {
			alert('종료일은 시작일보다 작을 수 없습니다.');
			return;
		}
	}

	$.ajax({
		url: '/primx/fun/pol/distribution/save.do',
		type: 'POST',
		data: {
			deptId: deptId,
			beginDate: beginDate,
			endDate: endDate,
			releaseYn: releaseYn,
			includeSubDept: includeSubDept
		},
		success: function(response) {
			if(response.status == 'success') {
				alert('배포 저장 성공');
				loadDistributionTable();
				$(".distributionPolicyPopup").addClass("hide");
			} else {
				alert('배포 저장 실패: ' + response.message);
			}
		},
		error: function(xhr, status, error) {
			console.error("배포 저장 실패:", error);
		}
	});
}
