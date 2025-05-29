/* 신청 및 결재 > 결재 */
var table = null;

/* csv 다운로드 제목포멧 */
var firstDepthTit = $(".left nav .firstDepthMenu > li.select > p").text();
var secondDepthTit = $(".left nav .firstDepthMenu > li.select .secondDepthMenu > li.select a").text();
var today = new Date();
var formattedDate = today.getFullYear() + '-' +
    ('0' + (today.getMonth() + 1)).slice(-2) + '-' +
    ('0' + today.getDate()).slice(-2);
    
async function pageInit(){
	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();

	/* dataTables 초기화 */
	table = initDataTables('#grid01');
	
	/* datepicker */
	datePicker();
	
	/** 목록 조회 */
	fnSearchList();

	/* 알림에서 페이지 접근시 왼쪽메뉴 셀렉트 되도록 */
	var firstDepth = $(".firstDepthMenu .approval");
	$(".left li").removeClass("select");
	$(firstDepth).closest("li").addClass("select open");
	$(".approvalLi").addClass("select");

	// 리스트 최초 로드시 롤권한 정보 로드
	const roleData = await loadRoleInfo(1);
	const targetSelectBox = $('.createRequestViewPopup select[name="userRole"]');
	targetSelectBox.empty();
	for(var i = 0; i < roleData.data.length; i++){
		targetSelectBox.append('<option value="'+ roleData.data[i].authorCode +'">'+ roleData.data[i].authorDescription +'</option>');
	}
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
	$('#csvExport').off('click');
	
	table = initDataTables('#grid01' ,{
		ajax: {
			url: '/primx/fun/rqt/requestApproval/list.do',
			type: 'POST',
			data: function(data) {
				data.startDt = $('#startDt').val();
				data.endDt = $('#endDt').val();
				data.requestApprovalCode = $('#approvalCode').val();
				data.resultCd = $('#resultCd').val();
				data.searchType = $('#searchType').val();
				data.searchWord = $('#searchWord').val();
			}
		},
		processing: true,
    	serverSide: true,
		language :{
			emptyTable: "No Data.",
		},
		columns: [
	        { data: null },
	        { data: 'requestCode' },
	        {
				data: null,
				render: function (data, type, row, meta) {
					return setFormName(data.requestApprovalCode);
				}
			},
	        {
				data: 'statusCd',
				render: function (data, type, row, meta) {
					return getStatusCdToValue(data);
				}
			},
			{
			    data: 'resultCd',
			    render: function (data, type, row, meta) {
			        // 결과 코드에 따라 클래스 추가
			        if (data === 'RE') { // 반려 상태
			            return '<span class="return">' + getResultCdToValue(data) + '</span>';
			        } else if (data === 'AP') { // 승인 상태
			            return '<span class="approval">' + getResultCdToValue(data) + '</span>';
			        } else { // 다른 상태
			            return '<span>' + getResultCdToValue(data) + '</span>';
			        }
			    },
			    createdCell: function (td, cellData, rowData, row, col) {
			        // td에 클래스 추가
			        $(td).addClass('result');
			    }
			},
	        { data: 'userNm' },
	        { data: 'title' },
	        {
				data: 'createdDate',
				render: function(data, type, row, meta) {
					return moment(data).format('YYYY-MM-DD');
				}
			},
	        {
				data: null,
				render: function (data, type, row, meta) {
					return getRequestDate(row, 'YYYY-MM-DD');
				}
			},
	        {
				data: 'resultDate',
				render: function(data, type, row, meta) {
					return data ? moment(data).format('YYYY-MM-DD') : '';
				}
			}
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
    $('#csvExport').on('click', function() {
        table.button('.buttons-csv').trigger();
    });

	// td 클릭시 상세보기 팝업
	$('#grid01 tbody').off('click');
	$('#grid01 tbody').on('click', 'td:not(:first-child)', function () {
		var table = $('#grid01').DataTable();
		var row = table.row(this).data();           // returns undefined
		viewData(row);
    });
}

/**
 * 신청일자 변환
 */
function getRequestDate(row, format) {
	var requestDate = moment(row.createdDate).format(format);
	// 수정일이 있는경우 수정일로 변경
	if (row.modifiedDate != null) {
		requestDate = moment(row.modifiedDate).format(format);
	}
	return requestDate;
}

/**
 * 상태 코드 변환
 */
function getStatusCdToValue(data) {
	var rtnStr = '-';
	if (data == 'CO') {
		rtnStr = '신청 완료';
	} else if (data == 'PE') {
		rtnStr = '대기';
	} else if (data == 'DR') {
		rtnStr = '진행중';
	}
	return rtnStr;
}

/**
 * 결과 코드 변환
 */
function getResultCdToValue(data) {
	var rtnStr = '-';
	if (data == 'AP') {
		rtnStr = '승인';
	} else if (data == 'RE') {
		rtnStr = '반려';
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

	initViewForm('view');
	
	$('#delegatedApprViewBtn').addClass('hide');
	$('#apprViewBtn').addClass('hide');

    $.ajax({
        url: '/primx/fun/rqt/requestApproval/info.do?requestId='+ rowData.requestId,
        method: 'GET',
        success: function (data) {
			// 신청 정보 세팅
			var req = data.request;
			var approvalUserId = data.approvalUserId;
			var approvalFlag = false;
			var delegatedFlag = false;
			$('#viewRequestCode').text(req.requestCode);
			$('input[name=viewRequestCode]').val(req.requestCode);
			$('input[name=viewRequestId]').val(req.requestId);
			$('#viewRequestDate').text(getRequestDate(req, 'YYYY-MM-DD'));

			$('#viewStatusCd').text(getStatusCdToValue(req.statusCd));
			$('#viewResultCd').text(getResultCdToValue(req.resultCd));
			$('#viewRequestUserNm').text(req.userNm);
			$('#viewRequestTitle').val(req.title);
			$('#viewRequestReason').val(req.reason);

            // 결재선 정보 세팅
            var appr = data.approvers;
            var apprData = {};
            $('#viewApprovers').empty();
            $('#viewApprovers').append('<li class="startLine" data-i18n="createRequest.createRequestPopup.approvalLine">결<br>재<br>선</li>');
            // 추가된 부분: 동적으로 생성된 HTML 요소에 다국어 적용
			applyTranslations();

            if (appr != null) {
	            for (var i = 0; i < appr.length; i++) {
					var userId = appr[i].emplyrId;
					var deptId = appr[i].deptId;
		            var deptNm = appr[i].deptName;
		            var userNm = appr[i].userNm;
					var userRole = appr[i].userRole;
		            var resultCd = getResultCdToValue(appr[i].resultCd);
		            var resultTime = appr[i].resultDate ? moment(appr[i].resultDate).format('HH:mm:ss') : '';
					$('#viewApprovers').append(genApprHtml(userId, deptId, deptNm, userNm, userRole, resultCd, resultTime));
				}

				// 내 전결처리 대상인지지 체크
				for (var i = 0; i < appr.length; i++) {
					if(approvalUserId == appr[i].emplyrId && (appr[i].approvalStatus == 1 || appr[i].approvalStatus == 2)) {
						delegatedFlag = true;
						apprData = appr[i];
						break;
					}
				}

				// 내 결재 순서인지 체크
				for (var i = 0; i < appr.length; i++) {
					if(approvalUserId == appr[i].emplyrId && appr[i].approvalStatus == 1) {
						approvalFlag = true;
						break;
					}
				}

				new DataTable('#apprGrid01', {
					layout: {
						topStart: null,
						topEnd: null,
						bottomStart: null,
						bottomEnd: null,
					},
					searching: false,
					ordering: false,
					language :{
						emptyTable: "데이터가 없습니다.",
					},
					destroy: true,
					columns: [
				        {
							data: 'resultCd',
							render: function (data, type, row, meta) {
								return getResultCdToValue(data);
							}
						},
				        { data: 'userNm' },
				        { data: 'deptName' },
				        { data: 'resultDate' },
				        { data: 'description' }
				    ],
				    data: appr
				});
			}

			// 공지사항 설정
			var userRole = appr[0].userRole;
			setNotice(userRole, '.createRequestViewPopup #approvalViewNotice');
			
			console.log("data: ", data);
			var requestUserApprover = data.requestUserApproval;
			
			// 서식 초기화
			$('.approvalFormTypeM').addClass('hide');
			$('.approvalFormTypeC').addClass('hide');
			$('.approvalFormTypeA').addClass('hide');
			// 서식 선택 값 세팅
			var selectedValue = req.requestApprovalCode;
			console.log("setFormName(selectedValue): ", setFormName(selectedValue));
			// div id viewRequestApprovalCodeName에 서식 이름 세팅
			$('.createRequestViewPopup #viewRequestApprovalCodeName').text(setFormName(selectedValue));
			

			$('#viewRequestApprovalCode').val(selectedValue);
			console.log("selectedValue: ", selectedValue);
			console.log("requestUserApprover: ", requestUserApprover);
			// 선택된 값에 따라 해당 wrap 표시
			if (selectedValue == 'M') {
				$('.approvalFormTypeM').removeClass('hide');
				$('#viewRequestReasonM').val(req.reason);
			} else if (selectedValue == 'C') {
				// datePicker에 날짜를 세팅한다
				$('.approvalFormTypeC .datepicker').val(setYYMMDD(requestUserApprover.expiredDate));
				$('#viewRequestReasonC').val(req.reason);
				$('.approvalFormTypeC').removeClass('hide');
			} else if (selectedValue == 'A') {
				$('.approvalFormTypeA input[name=userId]').val(requestUserApprover.userId);
				$('.approvalFormTypeA input[name=userName]').val(requestUserApprover.userName);
				$('.approvalFormTypeA input[name=email]').val(requestUserApprover.email);

				// 회사 정보 설정
				// 1. 초기화
				$('.approvalFormTypeA select[name=companyName]').empty();
				// 2. 값 세팅
				$('.approvalFormTypeA select[name=companyName]')
					.append('<option value="' + requestUserApprover.companyId + '">' 
					+ requestUserApprover.companyName + '</option>');
				$('.approvalFormTypeA select[name=companyName]').val(requestUserApprover.companyId);
				
				$('.approvalFormTypeA input[name=deptName]').val(requestUserApprover.deptName);
				$('.approvalFormTypeA input[name=deptId]').val(requestUserApprover.deptId);
				$('.approvalFormTypeA.select[name=userRole]').val(requestUserApprover.userRole);
				
				// expiredDate의 년도가 9999인 경우 무기한 아닌경우 기간 지정    
				if (requestUserApprover.expiredDate.substring(0, 4) == '9999') {
					$('.approvalFormTypeA .useDate .selectBox select').val(0);
				} else {
					$('.approvalFormTypeA .useDate .selectBox select').val(1);
				}
				$('.approvalFormTypeA .useDate .datepicker').val(setYYMMDD(requestUserApprover.expiredDate));
				$('#viewRequestReasonA').val(req.reason);
				$('.approvalFormTypeA').removeClass('hide');
			}
			// 등록 파일정보 세팅
			$('#viewFiles').empty();
			var files = data.files;
			if (files != null) {
			    for (var i = 0; i < files.length; i++) {
			        var fileDiv = $('<div></div>').text(files[i].fileName); // 파일 이름을 포함하는 div 생성
			        $('#viewFiles').append(fileDiv); // viewFiles 안에 div 추가
			    }
			}
			
			console.log("apprData.orderNumber: ", apprData.orderNumber);


			// 버튼 제어
			if (approvalFlag) {
				// 결재하기 버튼
				$('#apprViewBtn').removeClass('hide');

			}

			// 정보 확인 팝업
			$(".createRequestViewPopup").removeClass("hide");
			
			// 결재처리 버튼 클릭시 결재화면
			$(".createRequestViewPopup .approvalBtn").off("click");
			$(".createRequestViewPopup .approvalBtn").on("click",function() {
				$('#delegatedFlag').val(0);
				$('#approvalRequestId').val(req.requestId);
				$('#approvalUserId').val(approvalUserId);
				$('#approvalRequestCode').val(apprData.requestCode);
				$('#approvalOrderNumber').val(apprData.orderNumber);
				$('#approvalUserNm').text(apprData.userNm);

				$(".approvalProcessPopup").removeClass("hide");
			});
			
			console.log("delegatedFlag: ", delegatedFlag);
			console.log("selectedValue: ", selectedValue);
			console.log("req.resultCd: ", req.resultCd);
			console.log("apprData.orderNumber: ", apprData.orderNumber);

			// 결과가 반려 또는 승인이 아닌경우 전결처리 버튼 표시
			if(selectedValue == 'M' && req.resultCd != 'RE' && req.resultCd != 'AP' && delegatedFlag) {
				
				// 전결처리 버튼
				(async function() {
					if(await checkDelegatedYn()) {
						$('#delegatedApprViewBtn').removeClass('hide');
					}
				})();

				// 결재처리 버튼 클릭시 결재화면
				$(".createRequestViewPopup .delegatedApprovalBtn").off("click");
				$(".createRequestViewPopup .delegatedApprovalBtn").on("click",function() {
					$('#delegatedFlag').val(1);
					$('#approvalRequestId').val(req.requestId);
					$('#approvalUserId').val(approvalUserId);
					$('#approvalRequestCode').val('AP');
					$('#approvalOrderNumber').val(apprData.orderNumber);
					$('#approvalUserNm').text(apprData.userNm);
					$('#approvalDescription').val(apprData.userNm + '님이 전결처리 하였습니다.');
	
					saveApproval();
				});
			}
        },
        error: function (err) {
			alert('ajax call error');
        }
    });
}

// 전결 사용 여부 체크
async function checkDelegatedYn() {
    try {
        // 사용자 권한 로드
        const myInfo = await loadMyInfo();
        const userRole = myInfo.userRole;

        // 공지사항 로드
        const notice = await loadNotice();
        
        if (!notice || !notice.data || !Array.isArray(notice.data)) {
            console.error('공지사항 데이터가 올바르지 않습니다.');
            return false;
        }

        for(const item of notice.data) {
            if (item.requestUserRole === 'ADMIN' && userRole.includes('ADMIN') && item.delegatedYn === 'Y') {
                // console.log('전결 사용 여부: true');
                return true;
            }
            else if (item.requestUserRole === 'USER' && userRole.includes('USER') && item.delegatedYn === 'Y') {
                // console.log('전결 사용 여부: true');
                return true;
            }
        }
        // console.log('전결 사용 여부: false');
        return false;
    } catch (error) {
        console.error('전결 사용 여부 확인 중 오류 발생:', error);
        return false;
    }
}

/**
 * 상세화면 폼 초기화
 */
function initViewForm() {
	callPageClass = '.createRequestViewPopup';
	// 결재선 수정 여부 설정
	var approvalLineData = $('#viewApprovers').find('li');

	// 제목 비활성화
	$('#viewRequestTitle').attr("readonly", true).addClass("nowrite");
	// 사유 비활성화
	$('#viewRequestReasonM').attr("readonly", true);
	$('#viewRequestReasonC').attr("readonly", true);
	$('#viewRequestReasonA').attr("readonly", true);
	// 수정 버튼
	$('#modifiedBtn').removeClass('hide');
	// 결재선 추가 버튼
	$('.addLineDiv').addClass('hide');
	// li 커서 grab상태

	$('.lineWrap .otherDiv').removeClass('editLi').attr('draggable', false);
	// 회수 버튼
	$('#rejectedBtn').removeClass('hide');
	// 결재선 등록 버튼
	//$('#resultBtn').addClass('hide');
	// 저장 버튼
	$('#saveBtn').addClass('hide');

	$('#viewApprovers').find('li.registrationApprovalBtn').remove();
	setApprovalLineEdit(false, approvalLineData);
}

/**
 * 신청 정보 저장
 */
var clickCheck = false;
function saveApproval() {
	if (clickCheck){
		return;
	}
	var sendData = {
		orderNumber: 0,
		userId: '',
		requestId: '',
		requestCode: '',
		resultCd: '',
		description: '',
		delegatedFlag: 0
	};

	var userId = $('#approvalUserId').val();
	var requestId = $('#approvalRequestId').val();
	var requestCode = $('#approvalRequestCode').val();
	var orderNumber = $('#approvalOrderNumber').val();
	if (orderNumber == '' || userId == '' || requestId == '' || requestCode == '') {
		console.log("orderNumber: ", orderNumber);
		console.log("userId: ", userId);
		console.log("requestId: ", requestId);
		console.log("requestCode: ", requestCode);
		alert('결재 정보가 없습니다.');
		$(".approvalProcessPopup").addClass("hide");
		$(".createRequestViewPopup").addClass("hide");
		return false;
	}
	sendData.orderNumber = Number(orderNumber);
	sendData.userId = userId;
	sendData.requestId = Number(requestId);
	sendData.requestCode = requestCode;
	sendData.delegatedFlag = Number($('#delegatedFlag').val());

	var approval = $('input[name=approval]:checked').val();
	if (approval == '') {
		alert('결재처리를 선택하세요.');
		return false;
	}
	sendData.resultCd = approval;

	var desc = $.trim($('#approvalDescription').val());
	if (desc == '') {
		alert('결재처리의견을 입력하세요.');
		return false;
	}
	sendData.description = desc;
	clickCheck = true;
	console.log("sendData: ", sendData);
	// alert("sendData: " + sendData);
    $.ajax({
        url: '/primx/fun/rqt/requestApproval/modified.do',
        method: 'POST',
        data: sendData,
        success: function (data) {
			if (data.status == 'success') {
				fnSearchList();
				$(".approvalProcessPopup").addClass("hide");
				$(".createRequestViewPopup").addClass("hide");
				alert('결재처리가 완료되었습니다.');
			} else {
				alert("결재처리 실패 : " + data.message);
			}
			clickCheck = false;
        },
        error: function (err) {
			clickCheck = false;
			alert('ajax call error');
        }
    });
			
}

// 검색
function searchApprovalPage(popupName) {
	
    if(popupName == 'searchAreaPopup') {
		var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
		var searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
		var searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
		var searchApprovalCode = $(".searchAreaPopup .search_wrap select[name='approvalCode']");
		var searchResultCd = $(".searchAreaPopup .search_wrap select[name='resultCd']");
		var searchSearchType = $(".searchAreaPopup .search_wrap select[name='searchType']");
		var searchSearchWord = $(".searchAreaPopup .search_wrap input[name='searchWord']");
		
		if(searchDateType.val() != null && searchDateType.val() != "") {
			if(!searchFieldValid('date', searchStartDate.val(), searchEndDate.val(), searchDateType.val())) {
				return;
			}
			var filterStr = searchStartDate.val() +' ~ '+ searchEndDate.val();
			addFilterItem(".approvalPage .searchBox", "기간", filterStr, popupName);
		}
		else {
			resetSearchValue(popupName, "기간");
		}

		if(searchApprovalCode.val() != null && searchApprovalCode.val() != "") {
			// 선택된 값의 라벨을 추가
			addFilterItem(".approvalPage .searchBox", "신청 분류", searchApprovalCode.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "신청 분류");
		}

		if(searchResultCd.val() != null && searchResultCd.val() != "") {
			// 선택된 값의 라벨을 추가
			addFilterItem(".approvalPage .searchBox", "결과", searchResultCd.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "결과");
		}

		if(searchSearchType.val() != null && searchSearchType.val() != "" && searchSearchWord.val() != null && searchSearchWord.val() != "") {
			if(!searchFieldValid('condition', searchSearchWord.val(), null, null)) {
				return;
			}
			// 선택된 값의 라벨을 추가
			addFilterItem(".approvalPage .searchBox", "조건", searchSearchType.find('option:selected').text() + " [" + searchSearchWord.val() + "]", popupName);
		}
		else {
			resetSearchValue(popupName, "조건");
		}
		fnSearchList();
		$('.searchAreaPopup').addClass('hide');
	}
	else {
		console.log('approvalPage: no match popupName: ', popupName);
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
}

function resetSearchValue(popupName, filterTitle) {
    console.log('resetSearchValue: ', filterTitle);

    if(popupName == 'searchAreaPopup') {
		var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
		var searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
		var searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
		var searchApprovalCode = $(".searchAreaPopup .search_wrap select[name='approvalCode']");
		var searchResultCd = $(".searchAreaPopup .search_wrap select[name='resultCd']");
		var searchSearchType = $(".searchAreaPopup .search_wrap select[name='searchType']");
		var searchSearchWord = $(".searchAreaPopup .search_wrap input[name='searchWord']");

		if(filterTitle == "기간") {
			searchDateType.val(searchDateType.find('option[value=""]').val());
			searchStartDate.val('');
			searchEndDate.val('');
		}   
		else if(filterTitle == "신청 분류") {
			searchApprovalCode.val(searchApprovalCode.find('option[value=""]').val());
		}
		else if(filterTitle == "결과") {
			searchResultCd.val(searchResultCd.find('option[value=""]').val());
		}
		else if(filterTitle == "조건") {
			searchSearchType.val(searchSearchType.find('option[value=""]').val());
			searchSearchWord.val('');
		}

	}
	
}

function batchApproval_v1() {
	console.log("일괄 승인");
	var description = '일괄승인';

	// 선택된 row의 결과가 DR, AP 를 제외한 신청번호를 배열로 저장
	var selectedRows = [];
	$('.checkDataTable tbody tr.selected').each(function() {
		var resultCd = $(this).find('td:eq(4)').text();
		if(resultCd == '-') {
			var requestCode = $(this).find('td:eq(1)').text();
			selectedRows.push(requestCode);
		}
	});

	if(selectedRows.length == 0) {
		alert("승인할 신청번호를 선택하세요.");
		return;
	}
	console.log("selectedRows: ", selectedRows);

	// 일괄승인 ajax 호출
	$.ajax({
		url: '/primx/fun/rqt/requestApproval/batchApproval.do',
		method: 'POST',
		data: { requestCodes: selectedRows, description: description },
		success: function (data) {
			if (data.status === 'success') {
				console.log("일괄승인 완료");
				alert("일괄승인 완료");
				fnSearchList(); // 목록 새로고침
			} else {
				console.log("일괄승인 실패: " + data.message);
				alert("일괄승인 실패: " + data.message);
			}
		},
		error: function (err) {
			console.log("일괄승인 실패", err);
			alert("일괄승인 처리 중 오류가 발생했습니다.");
		}
	});
}
