/* 현황 > 이동파일 현황 */
var moveFileHistoryTable = null;
var originalFileInfoTable = null;
var moveFileUsageHistoryTable = null;

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

	/* input file 커스텀 */
	inputFileCustom();

	/* datepicker */
	datePicker();
	
	// CSV 내보내기 버튼과 연결 (한 번만 바인딩)
	$('#moveFileHistoryCsvExport').off('click').on('click', function() {
		if (moveFileHistoryTable) {
			moveFileHistoryTable.button('.buttons-csv').trigger();
		}
	});
	
	loadMoveFileHistoryTable();

	// 테이블 데이터 클릭 시 팝업 오픈 함수 호출(해당 row전달)
	$('#moveFileHistoryTable tbody').off('click').on("click", 'tr', function(){
		if (moveFileHistoryTable) {
			var rowData = moveFileHistoryTable.row(this).data();
			openMoveFileHistoryViewPopup(rowData);
		}
	});

	/* 이동파일현황tab 활성화 */
	$(".moveFileTabWrap .tabTitle li").on("click", function(){
		var moveFileTabNum = $(".moveFileTabWrap .tabTitle li").index($(this));//click하는 인덱스
		$(".moveFileTabWrap .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
		$(".moveFileTabWrap .tabCont > li:eq(" + (moveFileTabNum) + ")").addClass("on");
		$(".moveFileTabWrap .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
		$(this).addClass("on");
	});

	/* 왼쪽메뉴 셀렉트 되도록 */
	var firstDepth = $(".firstDepthMenu .current");
	$(".left li").removeClass("select");
	$(firstDepth).closest("li").addClass("select open");
	$(".moveFileStatusLi").addClass("select");
}

async function loadMoveFileHistoryTable() {
	var searchDTO = {
		dateSelect: $('#dateSelect').val(),
		startDt: $('#startDt').val(),
		endDt: $('#endDt').val(),
		status: $('#status').val(),
		condition: $('#condition').val(),
		searchKeyword: $('#searchKeyword').val()
	};

	// 기존 테이블이 있다면 제거
	if (moveFileHistoryTable) {
		moveFileHistoryTable.destroy();
		moveFileHistoryTable = null;
	}

	moveFileHistoryTable = await initDataTables('#moveFileHistoryTable', {
		ajax: {
			url: '/primx/fun/file/loadMoveFileHistory.do',
			type: 'POST',
			data: searchDTO,
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
			// 번호 파일id 상태 파일명 사이즈 원본파일수 잔여횟수 시스템명 사용자 등록일시 종료일
			{ data: 'rowId' },
			{ data: 'fileKey' },
			// 상태는 expireDate가 현재시간보다 크면 사용, 작으면 종료
			{ data: 'expireDate', render: function(data, type, row, meta) {
				return compareDate(data);
			}},
			{ data: 'fileName' },
			{ data: 'fileSize', render: function(data, type, row, meta) {
				return convertByteUnitWithComma(data);
			}},
			{ data: 'fileMapCount' },
			{ data: 'decryptCnt' },
			{ data: 'computerName' },
			{ data: 'userNm' },
			{ data: 'insertDate' },
			{ data: 'expireDate' }
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
		]		
	});
}

function openMoveFileHistoryViewPopup(rowData) {
	console.log(rowData);
	initMoveFileHistoryViewPopup();
	$("#insertDate").text(rowData.insertDate);
	$("#popupStatus").text(compareDate(rowData.expireDate));
	$("#computerName").text(rowData.computerName);
	$("#userNm").text(rowData.userNm);
	$("#fileKey").text(rowData.fileKey);
	$("#fileName").text(rowData.fileName);
	$("#decryptCnt").text(rowData.decryptCnt);
	$("#expireDate").text(rowData.expireDate);

	// 팝업 내 원본파일 조회
	loadOriginalFileInfoTable(rowData.fileKey);

	// 팝업 내 이동파일 사용 이력 조회
	loadMoveFileUsageHistoryTable(rowData.fileKey);

	
	// 팝업 내 탭 초기화
	$(".moveFileHistoryViewPopup .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
	$(".moveFileHistoryViewPopup .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
	$('.moveFileHistoryViewPopup .tabTitle li:first-child').addClass("on"); //첫번째 li에 테두리를 추가해라
	$('.moveFileHistoryViewPopup .tabCont > li:first-child').addClass("on"); //첫번째 탭을 선택해라


	$(".moveFileHistoryViewPopup").removeClass("hide");
}

// 상세 팝업 초기화
function initMoveFileHistoryViewPopup() {
	$("#insertDate").text("");
	$("#popupStatus").text("");
	$("#computerName").text("");
	$("#userNm").text("");
	$("#fileKey").text("");
	$("#fileName").text("");
	$("#decryptCnt").text("");
	$("#expireDate").text("");

	originalFileInfoTable = null;
	moveFileUsageHistoryTable = null;
}

// 날짜 비교 결과 리턴
function compareDate(data) {
	var expireDate = moment(data, 'YYYY-MM-DD HH:mm:ss').toDate();
	var currentDate = new Date();

	// console.log('data: ' + data);
	// console.log('new Date(): ' + currentDate);

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

	// // 기존 테이블 삭제
	// if ($.fn.DataTable.isDataTable('#moveFileUsageHistoryTable')) {
	// 	$('#moveFileUsageHistoryTable').DataTable().clear().destroy();
	// }

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




function searchMoveFileHistory(){
	// val의 값으로 조건을 추가하고, 선택된 val의 텍스트를 필터 타이틀로 사용한다
	var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
	var searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
	var searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
	var searchStatus = $(".searchAreaPopup .search_wrap select[name='status']");
	var searchCondition = $(".searchAreaPopup .search_wrap select[name='condition']");
	var searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']");
	

	if(searchDateType.val() != null && searchDateType.val() != "") {
		if(!searchFieldValid('date', searchStartDate.val(), searchEndDate.val(), searchDateType.val())) {
			return;
		}
		var filterStr = searchStartDate.val() +' ~ '+ searchEndDate.val();
		addFilterItem(".moveFileHistory .searchBox", "기간", filterStr, 'searchAreaPopup');
	}
	else {
		resetSearchValue("기간");
	}

	if(searchStatus.val() != null && searchStatus.val() != "") {
		addFilterItem(".moveFileHistory .searchBox", "상태", searchStatus.find('option:selected').text(), 'searchAreaPopup');
	}
	else {
        // select 선택을 초기화 option값이 공백인 값을 선택
		resetSearchValue("상태");
	}

	if(searchCondition.val() != null && searchCondition.val() != "" && searchKeyword.val() != null && searchKeyword.val() != "") {
		if(!searchFieldValid('condition', searchKeyword.val(), null, null)) {
			return;
		}
		addFilterItem(".moveFileHistory .searchBox", "조건", searchCondition.find('option:selected').text() + " [" + searchKeyword.val() + "]", 'searchAreaPopup');
	}
	else {
        // select 선택을 초기화 option값이 공백인 값을 선택
		resetSearchValue("조건");
	} 

    loadMoveFileHistoryTable();

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
    
    loadMoveFileHistoryTable();
}

function resetSearchValue(filterTitle) {
    console.log('resetSearchValue: ', filterTitle);

	var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
	var searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
	var searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
	var searchStatus = $(".searchAreaPopup .search_wrap select[name='status']");
	var searchCondition = $(".searchAreaPopup .search_wrap select[name='condition']");
	var searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']");
    
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

