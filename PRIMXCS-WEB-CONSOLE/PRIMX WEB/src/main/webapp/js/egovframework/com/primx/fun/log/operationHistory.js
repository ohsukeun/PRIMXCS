let dataTable = null;

/* csv 다운로드 제목포멧 */
var firstDepthTit = $(".left nav .firstDepthMenu > li.select > p").text();
var secondDepthTit = $(".left nav .firstDepthMenu > li.select .secondDepthMenu > li.select a").text();
var today = new Date();
var formattedDate = today.getFullYear() + '-' +
    ('0' + (today.getMonth() + 1)).slice(-2) + '-' +
    ('0' + today.getDate()).slice(-2);
    
/* 이력 > 운영이력 */
function pageInit(){
	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();

	/* input file 커스텀 */
	inputFileCustom();
	/* dataTables */
	initDataTable();

	/* datepicker */
	datePicker();
	
	// 이벤트 리스너 등록
	initEventListeners();
	
	/* 알림에서 페이지 접근시 왼쪽메뉴 셀렉트 되도록 */
	var firstDepth = $(".firstDepthMenu .history");
	$(".left li").removeClass("select");
	$(firstDepth).closest("li").addClass("select open");
	$(".operationHistoryLi").addClass("select");
	
}

function initEventListeners() {


	// 분류 선택에 따른 종류 옵션 변경
	$('select[name="searchCategory"]').on('change', function() {
		const category = $(this).val();
		const $typeSelect = $('select[name="searchType"]');
		
		// 기본 옵션 설정
		$typeSelect.empty().append('<option value="">전체</option>');
		
		// 선택된 분류에 따른 옵션 추가
		if(category === '연동') {
			$typeSelect.append('<option value="인사 연동">인사 연동</option>');
		} else if(category === '시스템') {
			$typeSelect.append('<option value="미접속 시스템 삭제">미접속 시스템 삭제</option>');
			$typeSelect.append('<option value="시스템 사용자 변경">시스템 사용자 변경</option>');
			$typeSelect.append('<option value="에이전트 삭제">에이전트 삭제</option>');
		} else if(category === '접속') {
			$typeSelect.append('<option value="관리자 로그인">관리자 로그인</option>');
			$typeSelect.append('<option value="사용자 로그인">사용자 로그인</option>');
		} else if(category === '정책') {
			$typeSelect.append('<option value="제품 정책 변경">제품 정책 변경</option>');
			$typeSelect.append('<option value="제품 정책 적용">제품 정책 적용</option>');
			$typeSelect.append('<option value="시스템 종류 변경">시스템 종류 변경</option>');
			$typeSelect.append('<option value="PRIMX Agent 정책 적용">PRIMX Agent 정책 적용</option>');
		} else if(category === '관리자') {
			$typeSelect.append('<option value="관리자 추가">관리자 추가</option>');
			$typeSelect.append('<option value="관리자 삭제">관리자 삭제</option>');
		} else if(category === '사용자') {
			$typeSelect.append('<option value="수동 사용자 추가">수동 사용자 추가</option>');
			$typeSelect.append('<option value="수동 사용자 삭제">수동 사용자 삭제</option>');
		} else if(category === '업데이트') {
			$typeSelect.append('<option value="PRIMX Agent 업데이트">PRIMX Agent 업데이트</option>');
			$typeSelect.append('<option value="제품 업데이트">제품 업데이트</option>');
		}
	});
}

function initDataTable() {
	// 기존 테이블이 있다면 제거
	if (dataTable) {
		dataTable.destroy();
		dataTable = null;
	}

	// 기존 CSV 버튼 이벤트 바인딩 제거
	$('#operationHistoryCsvExport').off('click');
	console.log('initDataTable');
	dataTable = initDataTables('#dataTable' ,{
		processing: true,
		serverSide: true,
		ajax: {
			url: '/primx/fun/log/manageLog/list.do',
			type: 'POST',
			data: function(data) {
				data.searchDateType = $(".searchAreaPopup select[name='dateSelect']").val();
				data.searchStartDate = $(".searchAreaPopup input[name='searchStartDate']").val();
				data.searchEndDate = $(".searchAreaPopup input[name='searchEndDate']").val();
				data.searchCategory = $(".searchAreaPopup select[name='searchCategory']").val();
				data.searchType = $(".searchAreaPopup select[name='searchType']").val();
				data.searchCondition = $(".searchAreaPopup select[name='searchCondition']").val();
				data.searchKeyword = $(".searchAreaPopup input[name='searchKeyword']").val()
			}
		},
		columns: [
			{ data: null, defaultContent: '' },
			{ data: 'logDate' },
			{ data: 'logTime' },
			{ data: 'logTypeName' },
			{ data: 'logName' },
			{ data: 'message' },
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
	
	// CSV 내보내기 버튼과 연결
    $('#operationHistoryCsvExport').on('click', function() {
        dataTable.button('.buttons-csv').trigger();
    });
	
	$('#dataTable tbody').off('click');
	$('#dataTable tbody').on('click', 'td:not(:first-child)', function() {
		const data = dataTable.row(this).data(); // 클릭한 행의 데이터 가져오기
		if (data) {
			loadOperationHistoryDetails(data); // 사용자 정보를 로드하는 함수 호출
		}
	});
}

function loadOperationHistoryDetails(data) {
	// 팝업의 필드에 데이터 채우기
	$(".operationHistoryViewPopup .pop_cont .item em").each(function() {
		const fieldKey = $(this).attr("data-i18n"); // data-i18n 속성값을 가져오기

		switch (fieldKey) {
			case "operationHistory.operationHistoryViewPopup.dateTime":
				$(this).next().text(data.logDate + ' ' + data.logTime);
				break;
			case "operationHistory.operationHistoryViewPopup.category":
				// logTypeName이 없으면 빈 문자열 출력
				if(data.logTypeName == null || data.logTypeName == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.logTypeName);
				}
				break;
			case "operationHistory.operationHistoryViewPopup.kind":
				// logName이 없으면 빈 문자열 출력
				if(data.logName == null || data.logName == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.logName);
				}
				break;
			case "operationHistory.operationHistoryViewPopup.content":
				// message가 없으면 빈 문자열 출력
				if(data.message == null || data.message == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.message);
				}
				break;
			case "operationHistory.operationHistoryViewPopup.systemName":
				// computerName이 없으면 빈 문자열 출력
				if(data.computerName == null || data.computerName == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.computerName);
				}
				break;
			case "operationHistory.operationHistoryViewPopup.userName":
				// userName이 없으면 빈 문자열 출력
				if(data.userName == null || data.userName == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.userName);
				}
				break;
			case "operationHistory.operationHistoryViewPopup.workerName":
				// manageName이 없으면 빈 문자열 출력
				if(data.manageName == null || data.manageName == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.manageName);
				}
				break;
			default:
				break;
		}
	});

	// 팝업 표시
	$(".operationHistoryViewPopup").removeClass("hide");
}


function searchOperationHistory(){
	// val의 값으로 조건을 추가하고, 선택된 val의 텍스트를 필터 타이틀로 사용한다
	var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
	var searchStartDate = $(".searchAreaPopup .search_wrap input[name='searchStartDate']");
	var searchEndDate = $(".searchAreaPopup .search_wrap input[name='searchEndDate']");
	var searchCategory = $(".searchAreaPopup .search_wrap select[name='searchCategory']");
	var searchType = $(".searchAreaPopup .search_wrap select[name='searchType']");
	var searchCondition = $(".searchAreaPopup .search_wrap select[name='searchCondition']");
	var searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']");
	

	if(searchDateType.val() != null && searchDateType.val() != "") {
		if(!searchFieldValid('date', searchStartDate.val(), searchEndDate.val(), searchDateType.val())) {
			return;
		}
		var filterStr = searchStartDate.val() +' ~ '+ searchEndDate.val();
		addFilterItem(".operationHistory .searchBox", "기간", filterStr, 'searchAreaPopup');
	}
	else {
		resetSearchValue("기간");
	}

	if(searchCategory.val() != null && searchCategory.val() != "") {
		addFilterItem(".operationHistory .searchBox", "분류", searchCategory.find('option:selected').text(), 'searchAreaPopup');
	}
	else {
		resetSearchValue("분류");
	}

	if(searchType.val() != null && searchType.val() != "") {
		addFilterItem(".operationHistory .searchBox", "종류", searchType.find('option:selected').text(), 'searchAreaPopup');
	}
	else {
		resetSearchValue("종류");
	}

	if(searchCondition.val() != null && searchCondition.val() != "" && searchKeyword.val() != null && searchKeyword.val() != "") {
		if(!searchFieldValid('condition', searchKeyword.val(), null, null)) {
			return;
		}
		addFilterItem(".operationHistory .searchBox", "조건", searchCondition.find('option:selected').text() + " [" + searchKeyword.val() + "]", 'searchAreaPopup');
	}
	else {
		resetSearchValue("조건");
	} 

    initDataTable();

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
    
    initDataTable();
}

function resetSearchValue(filterTitle) {
    console.log('resetSearchValue: ', filterTitle);

	var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
	var searchStartDate = $(".searchAreaPopup .search_wrap input[name='searchStartDate']");
	var searchEndDate = $(".searchAreaPopup .search_wrap input[name='searchEndDate']");
	var searchCategory = $(".searchAreaPopup .search_wrap select[name='searchCategory']");
	var searchType = $(".searchAreaPopup .search_wrap select[name='searchType']");
	var searchCondition = $(".searchAreaPopup .search_wrap select[name='searchCondition']");
	var searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']");
    
    if(filterTitle == "기간") {
        searchDateType.val(searchDateType.find('option[value=""]').val());
        searchStartDate.val('');
        searchEndDate.val('');
    }   
    else if(filterTitle == "분류") {
        searchCategory.val(searchCategory.find('option[value=""]').val());
    }
    else if(filterTitle == "종류") {
        searchType.val(searchType.find('option[value=""]').val());
    }
    else if(filterTitle == "조건") {
        searchCondition.val(searchCondition.find('option[value="all"]').val());
        searchKeyword.val('');
    }
    
}
