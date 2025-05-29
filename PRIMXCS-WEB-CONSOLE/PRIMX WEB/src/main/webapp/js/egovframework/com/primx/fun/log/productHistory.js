let dataTable;

/* csv 다운로드 제목포멧 */
var firstDepthTit = $(".left nav .firstDepthMenu > li.select > p").text();
var secondDepthTit = $(".left nav .firstDepthMenu > li.select .secondDepthMenu > li.select a").text();
var today = new Date();
var formattedDate = today.getFullYear() + '-' +
    ('0' + (today.getMonth() + 1)).slice(-2) + '-' +
    ('0' + today.getDate()).slice(-2);
    
/* 이력 > 제품 이력 */
function pageInit(){

	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();

	/* input file 커스텀 */
	//inputFileCustom();
	
	/* dataTables 초기화 */
	dataTable = initDataTables('#dataTable');
	initDataTable();
	
	/* datepicker */
	datePicker();
	
	/* 이벤트 리스너 등록 */
	//initEventListeners();

	/* 왼쪽메뉴 셀렉트 되도록 */
	var firstDepth = $(".firstDepthMenu .history");
	$(".left li").removeClass("select");
	$(firstDepth).closest("li").addClass("select open");
	$(".productHistoryLi").addClass("select");

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

function initDataTable() {
	// 기존 테이블이 있다면 제거
	if (dataTable) {
		dataTable.destroy();
		dataTable = null;
	}

	// 기존 CSV 버튼 이벤트 바인딩 제거
	$('#productHistoryCsvExport').off('click');

	dataTable = initDataTables('#dataTable',{
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
					searchDateType: $(".searchAreaPopup select[name='dateSelect']").val(),
					searchStartDate: $(".searchAreaPopup input[name='searchStartDate']").val(),
					searchEndDate: $(".searchAreaPopup input[name='searchEndDate']").val(),
					searchLevel: $(".searchAreaPopup select[name='searchLevel']").val(),
					searchProduct: $(".searchAreaPopup select[name='searchProduct']").val(),
					searchType: $(".searchAreaPopup select[name='searchType']").val(),
					searchCondition: $(".searchAreaPopup select[name='searchCondition']").val(),
					searchKeyword: $(".searchAreaPopup input[name='searchKeyword']").val()
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
	
	// CSV 내보내기 버튼과 연결
    $('#productHistoryCsvExport').on('click', function() {
        dataTable.button('.buttons-csv').trigger();
    });

	$('#dataTable tbody').off('click');
	$('#dataTable tbody').on('click', 'td:not(:first-child)', function() {
		const data = dataTable.row(this).data(); // 클릭한 행의 데이터 가져오기
		if (data) {
			loadProductHistoryDetails(data); // 사용자 정보를 로드하는 함수 호출
		}
	});
}

function loadProductHistoryDetails(data) {
	// 팝업의 필드에 데이터 채우기
	$(".productHistoryViewPopup .pop_cont .item em").each(function() {
		const fieldKey = $(this).attr("data-i18n"); // data-i18n 속성값을 가져오기

		switch (fieldKey) {
			case "productHistory.productHistoryViewPopup.dateTime":
				$(this).next().text(data.logDate + ' ' + data.logTime);
				break;
			case "productHistory.productHistoryViewPopup.level":
				// logLevel이 없으면 빈 문자열 출력
				if(data.logLevel == null || data.logLevel == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.logLevel);
				}
				break;
			case "productHistory.productHistoryViewPopup.product":
				// productName이 없으면 빈 문자열 출력
				if(data.productName == null || data.productName == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.productName);
				}
				break;
			case "productHistory.productHistoryViewPopup.kind":
				// logName이 없으면 빈 문자열 출력
				if(data.logName == null || data.logName == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.logName);
				}
				break;
			case "productHistory.productHistoryViewPopup.content":
				// logText가 없으면 빈 문자열 출력
				if(data.logText == null || data.logText == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.logText);
				}
				break;
			case "productHistory.productHistoryViewPopup.systemName":
				// computerName이 없으면 빈 문자열 출력
				if(data.computerName == null || data.computerName == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.computerName);
				}
				break;
			case "productHistory.productHistoryViewPopup.userName":
				// userName이 없으면 빈 문자열 출력
				if(data.userName == null || data.userName == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.userName);
				}
				break;
			case "productHistory.productHistoryViewPopup.workerName":	
				// workerName이 없으면 빈 문자열 출력
				if(data.workerName == null || data.workerName == "") {
					$(this).next().text("");
				} else {
					$(this).next().text(data.workerName);
				}
				break;
			default:
				break;
		}
	});

	// 팝업 표시
	$(".productHistoryViewPopup").removeClass("hide");
}



function searchProductHistory(){
	// val의 값으로 조건을 추가하고, 선택된 val의 텍스트를 필터 타이틀로 사용한다
	var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
	var searchStartDate = $(".searchAreaPopup .search_wrap input[name='searchStartDate']");
	var searchEndDate = $(".searchAreaPopup .search_wrap input[name='searchEndDate']");
	var searchLevel = $(".searchAreaPopup .search_wrap select[name='searchLevel']");
	var searchProduct = $(".searchAreaPopup .search_wrap select[name='searchProduct']");
	var searchType = $(".searchAreaPopup .search_wrap select[name='searchType']");
	var searchCondition = $(".searchAreaPopup .search_wrap select[name='searchCondition']");
	var searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']");
	

	if(searchDateType.val() != null && searchDateType.val() != "") {
		if(!searchFieldValid('date', searchStartDate.val(), searchEndDate.val(), searchDateType.val())) {
			return;
		}
		var filterStr = searchStartDate.val() +' ~ '+ searchEndDate.val();
		addFilterItem(".productHistory .searchBox", "기간", filterStr, 'searchAreaPopup');
	}
	else {
		resetSearchValue("기간");
	}

	if(searchLevel.val() != null && searchLevel.val() != "") {
		addFilterItem(".productHistory .searchBox", "레벨", searchLevel.find('option:selected').text(), 'searchAreaPopup');
	}
	else {
		resetSearchValue("레벨");
	}

	if(searchProduct.val() != null && searchProduct.val() != "") {
		addFilterItem(".productHistory .searchBox", "제품", searchProduct.find('option:selected').text(), 'searchAreaPopup');
	}
	else {
		resetSearchValue("제품");
	}  
		
	if(searchType.val() != null && searchType.val() != "") {
		addFilterItem(".productHistory .searchBox", "종류", searchType.find('option:selected').text(), 'searchAreaPopup');
	}
	else {
		resetSearchValue("종류");
	}

	if(searchCondition.val() != null && searchCondition.val() != "" && searchKeyword.val() != null && searchKeyword.val() != "") {
		if(!searchFieldValid('condition', searchKeyword.val(), null, null)) {
			return;
		}
		addFilterItem(".productHistory .searchBox", "조건", searchCondition.find('option:selected').text() + " [" + searchKeyword.val() + "]", 'searchAreaPopup');
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
	var searchLevel = $(".searchAreaPopup .search_wrap select[name='searchLevel']");
	var searchProduct = $(".searchAreaPopup .search_wrap select[name='searchProduct']");
	var searchType = $(".searchAreaPopup .search_wrap select[name='searchType']");
	var searchCondition = $(".searchAreaPopup .search_wrap select[name='searchCondition']");
	var searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']");
    
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