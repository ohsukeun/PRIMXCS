/* 신청 및 결재 > 신청 */
var requestTable = null;
var moveFileListTable = null;
var deptUserTable = null;

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
		
	/* dataTables 초기화 */
	requestTable = initDataTables('#grid01');
	deptUserTable = initDataTables('#deptUserTable');
	
	/*이동파일 조회 팝업*/
	$(".createRequestPopup .moveFileListSearchBtn").on("click",function(){
		$(".moveFileListSearchPopup").removeClass("hide");
	})
	
	/* datepicker */
	datePicker();
	
	/* 목록 조회 */
	fnSearchList(requestTable, '#grid01', null, clickViewData);
	
	// 초기화
	callPageClass = '';

	initUserEventListeners();    

	setMenuBtn('#menuRegistrationBtn');
	
	loadAlarm();

	/* 신청서 서식 선택 */
	$('.typeOfApplication select').on('change', function () {
        const selectedValue = $(this).val();
		console.log(selectedValue);

        // 모든 wrap을 숨기기
        $('.approvalFormTypeM').addClass('hide');
        $('.approvalFormTypeC').addClass('hide');
        $('.approvalFormTypeA').addClass('hide');

        // 선택된 값에 따라 해당 wrap 표시
        if (selectedValue == 'M') {
            $('.approvalFormTypeM').removeClass('hide');
            $('#moveFielViewBtn').removeClass('hide');
        } else if (selectedValue == 'C') {
            $('.approvalFormTypeC').removeClass('hide');
            $('#moveFielViewBtn').addClass('hide');
        } else if (selectedValue == 'A') {
            $('.approvalFormTypeA').removeClass('hide');
            $('#moveFielViewBtn').addClass('hide');
        }
    });
    
    // 신청버튼 클릭 시 팝업 오픈(상세정보)
	$(".createRequest .registrationBtn").on("click",function(){
		$('.popup.createRequestPopup.innerVerPop').removeClass('hide');
	})

	/* 왼쪽메뉴 셀렉트 되도록 */
	var firstDepth = $(".firstDepthMenu .approval");
	$(".left li").removeClass("select");
	$(firstDepth).closest("li").addClass("select open");
	$(".createRequestLi").addClass("select");
}

function listReload(){
	fnSearchList(requestTable, '#grid01', null, clickViewData);
}

/**
 * 목록 조회
 */

function fnSearchList(isTable, targetTable, searchOption, clickEventHandler) {

	// 기존 CSV 버튼 이벤트 바인딩 제거
	$('#csvExport').off('click');
	
	console.log('table 초기화');
	isTable = initDataTables(targetTable ,{
		ajax: {
			url: '/primx/fun/rqt/createRequest/list.do',
			type: 'POST',
			data: function(data) {
				data.startDt = $('#startDt').val();
				data.endDt = $('#endDt').val();
				data.requestApprovalCode = $('#approvalCode').val();
				data.statusCd = $('#statusCd').val();
				data.resultCd = $('#resultCd').val();
				data.searchType = $('#searchType').val();
				data.searchWord = $('#searchWord').val();
				// 신청 분류 - 이동파일에 대한 조회인 경우
				if(searchOption != null){
					data.startDt = searchOption.startDt;
					data.endDt = searchOption.endDt;
					data.requestApprovalCode = searchOption.requestApprovalCode;
					data.statusCd = searchOption.statusCd;
					data.resultCd = searchOption.resultCd;
					data.searchType = searchOption.searchType;
					data.searchWord = searchOption.searchWord;
				}
			}
		},
		processing: true,
    	serverSide: true,
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
        if (isTable) {
            isTable.button('.buttons-csv').trigger();
        }
    });

	clickEventHandler(targetTable);
}

function clickViewData(targetTable){
	
	// td 클릭시 상세보기 팝업
	$(targetTable + ' tbody').off('click');
	$(targetTable + ' tbody').on('click', 'td:not(:first-child)', function () {
		var table = $(targetTable).DataTable();
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

    $.ajax({
        url: '/primx/fun/rqt/requestApproval/info.do?requestId='+ rowData.requestId,
        method: 'GET',
        success: function (data) {
			console.log("data: ", data);
			
			// 신청 정보 세팅
			var req = data.request;
			$('#viewRequestCode').text(req.requestCode);
			$('input[name=viewRequestCode]').val(req.requestCode);
			$('input[name=viewRequestId]').val(req.requestId);
			$('#viewRequestDate').text(getRequestDate(req, 'YYYY-MM-DD'));
			$('#viewStatusCd').text(getStatusCdToValue(req.statusCd));
			$('#viewResultCd').text(getResultCdToValue(req.resultCd));
			$('#viewRequestUserNm').text(req.userNm);
			$('#viewRequestTitle').val(req.title);

            // 결재선 정보 세팅
            var appr = data.approvers;
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
						emptyTable: "No Data.",
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
			setNotice(userRole, '.createRequestViewPopup #createRequestViewNotice');
			
			console.log("data: ", data);
			var requestUserApprover = data.requestUserApproval;
			
			// 서식 초기화
			$('.approvalFormTypeM').addClass('hide');
			$('.approvalFormTypeC').addClass('hide');
			$('.approvalFormTypeA').addClass('hide');
			// 서식 선택 값 세팅
			var selectedValue = req.requestApprovalCode;
			$('#viewRequestApprovalCode').val(selectedValue);
			console.log("selectedValue: ", selectedValue);
			console.log("requestUserApprover: ", requestUserApprover);
			// 선택된 값에 따라 해당 wrap 표시

			$('.createRequestViewPopup #viewRequestApprovalCodeName').text(setFormName(selectedValue));
			
			if (selectedValue == 'M') {
				$('.approvalFormTypeM').removeClass('hide');
				$('#viewRequestReasonM').val(req.reason);
			} else if (selectedValue == 'C') {
				// datePicker에 날짜를 세팅한다
				console.log("requestUserApprover.expiredDate: ", requestUserApprover.expiredDate);
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
						
				// 버튼 제어 (null 또는 빈문자열인 경우 버튼 숨기기)
			if (req.statusCd != 'PE') {
				// 수정 버튼
				$('#modifiedBtn').addClass('hide');
				// 회수 버튼
				$('#rejectedBtn').addClass('hide');
			} else if(req.userId != data.approvalUserId) {
				$('#modifiedBtn').addClass('hide');
				$('#rejectedBtn').addClass('hide');
			}


			// 정보 확인 팝업
			$(".createRequestViewPopup").removeClass("hide");
	        },
	        error: function (err) {
				alert('ajax call error');
	        }
	    });
	}

/**
 * 상세화면 폼 초기화
 */
async function initViewForm(type) {
	callPageClass = '.createRequestViewPopup';
	// 결재선 수정 여부 설정
	var approvalLineData = $('#viewApprovers').find('li');

	if (type == 'edit') {
		// 제목 활성화
		$('#viewRequestTitle').attr("readonly", false).removeClass("nowrite");
		// 사유 활성화
		$('#viewRequestReasonM').attr("readonly", false);
		$('#viewRequestReasonC').attr("readonly", false);
		$('#viewRequestReasonA').attr("readonly", false);
		// 수정 버튼
		$('#modifiedBtn').addClass('hide');

		// 결재선 추가 버튼

		$('.addLineDiv').removeClass('hide');
		// 회수 버튼
		$('#rejectedBtn').addClass('hide');
		// 저장 버튼
		$('#saveBtn').removeClass('hide');

		// 항상 마지막에 버튼 추가
		$('#viewApprovers').append(createApprovalAddBtn());
		setApprovalLineEdit(true, approvalLineData);
		
	} else {
		console.log('initViewForm 호출');
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
		//$('#rejectedBtn').removeClass('hide');
		// 결재선 등록 버튼
		//$('#resultBtn').addClass('hide');
		// 저장 버튼
		$('#saveBtn').addClass('hide');

		$('#viewApprovers').find('li.registrationApprovalBtn').remove();
		setApprovalLineEdit(false, approvalLineData);
	}

	
	// 리스트 최초 로드시 롤권한 정보 로드
	const roleData = await loadRoleInfo(1);
	const targetSelectBox = $('.createRequestViewPopup select[name="userRole"]');
	targetSelectBox.empty();
	for(var i = 0; i < roleData.data.length; i++){
		targetSelectBox.append('<option value="'+ roleData.data[i].authorCode +'">'+ roleData.data[i].authorDescription +'</option>');
	}
}

/**
 * 신청 정보 저장
 */
var clickCheck = false;
async function modifiedRequest(status) {

	if (clickCheck){
		return;
	}
	var sendData = {
		requestId: 0,
		requestCode: '',
		statusCd: status,
		title: '',
		reason: '',
		apprChg: false,
		approvalLines: []
	};

	var requestId = $.trim($('input[name=viewRequestId]').val());
	var requestCode = $.trim($('input[name=viewRequestCode]').val());
	if (requestId == '' || requestCode == '') {
		alert('신청 정보가 없습니다.');
		$(".createRequestViewPopup").addClass("hide");
		return false;
	}
	sendData.requestId = Number(requestId);
	sendData.requestCode = requestCode;
	
	// 신청서 수정인 경우 처리
	if (status == 'PE') {
		
		var title = $.trim($('#viewRequestTitle').val());
		var reason;
		// 서식 선택 값 가져오기
		var selectedValue = $('#viewRequestApprovalCode').val();
		if(selectedValue == 'M') {
			reason = $.trim($('#viewRequestReasonM').val());
		}
		else if(selectedValue == 'C') {
			reason = $.trim($('#viewRequestReasonC').val());
		}
		else if(selectedValue == 'A') {
			reason = $.trim($('#viewRequestReasonA').val());
		}
		
		if (title == '') {
			alert('제목을 입력하세요.');
			return false;
		}
		sendData.title = title;
		if (reason == '') {
			alert('사유를 입력하세요.');
			return false;
		}
		sendData.reason = reason;
		
		// 결재선 세팅 필요
		// 결재선 변경인경우 apprChg = true 변경후 데이터 세팅
	}
	
	// 결재선 추가 버튼 제거
	$('#viewApprovers').find('li.registrationApprovalBtn').remove();
	// 결재선에 등록된 리스트 정보 가져오기
	var approvalLines = [];
	$('.popup.createRequestViewPopup .pop_cont .lineWrap li').each(function() {
		if (!$(this).hasClass('startLine') && !$(this).hasClass('addLineDiv')) {
			var approvalLine = {
				orderNumber: $(this).index() - 1,
				userId: $(this).find('input[name="approvalUserId"]').val(),
				userRole: $(this).find('input[name="approvalUserRole"]').val()
			};
			approvalLines.push(approvalLine);
		}
	});
	console.log('approvalLines: ', approvalLines);

	// 결재선 최소 2개 이상 체크
	if (approvalLines.length < 2) {
		$('#viewApprovers').append(createApprovalAddBtn());
	    alert('결재선을 등록해 주세요.');
	    return;
	}

	var approvalTarget = await setApprovalTarget(approvalLines[0].userRole, selectedValue);

	console.log('approvalTarget: ', approvalTarget);
	
	if(approvalTarget == 'A') {
		// 마지막 결재자가 
		if(!approvalLines[approvalLines.length-1].userRole.includes('ADMIN')) {
			$('#viewApprovers').append(createApprovalAddBtn());
			alert('최종 결재자는 관리자 여야 합니다.');
			return;
		}
	}
	else if(approvalTarget == 'U') {
		// 마지막 결재자가 
		if(!approvalLines[approvalLines.length-1].userRole.includes('USER')) {
			$('#viewApprovers').append(createApprovalAddBtn());
			alert('최종 결재자는 사용자 여야 합니다.');
			return;
		}
	}

	sendData.approvalLines = approvalLines;

	clickCheck = true;

    $.ajax({
        url: '/primx/fun/rqt/createRequest/modified.do',
        method: 'POST',
		contentType: 'application/json',
        data: JSON.stringify(sendData),
        success: function (response) {
			if (response.status == 'success') {
				fnSearchList(requestTable, '#grid01', null, clickViewData);
				$(".createRequestViewPopup").addClass("hide");
				var msg = status == 'PE' ? '수정이' : '회수가';
				alert('신청서 '+ msg +' 완료되었습니다.');
			} else {
				alert("신청서 수정 실패 : " + response.message);
			}
			clickCheck = false;
        },
        error: function (err) {
			clickCheck = false;
			alert('ajax call error');
        }
    });
}

/**
 * 상세화면 닫기
 */
function closeViewForm() {
	// 결재선 추가 버튼 제거
	$('#viewApprovers').find('li.registrationApprovalBtn').remove();
	$(".createRequestViewPopup").addClass("hide");
}


function handleDeptSearch() {
	var searchKeyword = $(".departSearchPopup input[name='searchKeyword']").val();
	loadDeptTree(searchKeyword);
	$(".departSearchPopup").addClass("hide");
}


/**
 * 신청 팝업 오픈
 */
async function openCreateRequestPopup() {
	callPageClass = '.createRequestPopup.innerVerPop';
    // 신청일자 추가 년월일 시분초
    $('#requestDate').text(moment().format('YYYY-MM-DD HH:mm:ss'));

    try {
        // 사용자 정보 로드 - await를 사용하여 비동기 처리
        const myInfo = await loadMyInfo();
        console.log('myInfo:', myInfo);

		// 신청 팝업 초기화
		clearCreateRequestPopup(myInfo);

		// 기본값 서식을 이동파일 생성 신청으로 지정
		// $('#requestApprovalCode').val('M');
        
        // 사용자 이름 설정
        $('#requestUserNm').text(myInfo.userName);
        $('#requestUserId').val(myInfo.emplyrId);

		console.log("myInfo: ", myInfo);
		console.log("loadCompanyInfo 호출 전");
        // 서식 : 사용자 등록 신청 시
        // 회사 정보 로드
        loadCompanyInfo($('.createRequestPopup #companyId'));
		
		
		const roleData = await loadRoleInfo(1);
		const targetSelectBox = $('.createRequestPopup select[name="userRole"]');
		targetSelectBox.empty();
		for(var i = 0; i < roleData.data.length; i++){
			targetSelectBox.append('<option value="'+ roleData.data[i].authorCode +'">'+ roleData.data[i].authorDescription +'</option>');
		}
		
		console.log("loadCompanyInfo 호출 후");
        // 롤권한 정보 로드
        setNotice(myInfo.userRole, '.createRequestPopup #createRequestPopupNotice');
        setReason(myInfo.userRole, '.createRequestPopup #requestReason');
		setFormUseYn(myInfo.userRole, '.createRequestPopup');

		// 신청 팝업 날짜 피커 이벤트 설정
		createRequestPopupDatePickerEvent();
    } catch (error) {
        console.error('사용자 정보 로드 실패:', error);
        alert('사용자 정보를 불러오는데 실패했습니다.');
    }

}

function createRequestPopupDatePickerEvent() {

    // datepicker 초기화
    $(".datepicker").datepicker({
        dateFormat: "yy-mm-dd", // 원하는 형식으로 지정
        onSelect: function(dateText, inst) {
            $(this).datepicker('hide'); // 날짜 선택 후 datepicker 닫기
        }
    });

	// SelectBox 변경 이벤트 설정
    $(".dateSelectSetData2").on("change", function () {
        var $selectBox = $(this); // 현재 선택된 selectBox
		var $usePeriodEnd = $("#accountExtensionPeriodEnd");

        var selectedValue = $selectBox.val(); // 선택된 값 (3, 6, 9, 12)
        var today = new Date(); // 오늘 날짜
        var endDate = ""; // 종료일 초기화
		console.log("selectedValue: ", selectedValue);
        if(selectedValue == '') {
			$usePeriodEnd.attr('readonly', false);
            endDate = "";
        } else {
			$usePeriodEnd.attr('readonly', true);
            var pastDate = new Date(today);
            pastDate.setMonth(today.getMonth() + parseInt(selectedValue)); // 선택된 값만큼 이전
            endDate = $.datepicker.formatDate("yy-mm-dd", pastDate); 
			console.log("endDate: ", endDate);
        }
		$usePeriodEnd.val(endDate);
    });
}


function clickEventHandler(date) {
    // .approvalListPopup 내의 .lineWrap을 대상으로 설정
    var targetSelector = '.popup.approvalListPopup.innerVerPop .pop_cont .lineWrap';

    // approvalLineData에 editLi 클래스를 추가하여 .approvalListPopup에만 적용
    var approvalLineData = genApprHtml(date.emplyrId, date.deptId, date.deptName, date.userNm, date.userRole, '-', '');

	// targetSelector에 approvalUserId 값이 date.emplyrId와 같은 경우 알림 표시
    var isUserAlreadyAdded = false;
    $(targetSelector).find('li').each(function() {
        var existingUserId = $(this).find('input[name="approvalUserId"]').val();
        if (existingUserId === date.emplyrId) {
            isUserAlreadyAdded = true;
            return false; // 반복문 종료
        }
    });

    if (isUserAlreadyAdded) {
        alert('이미 결재선에 추가된 사용자입니다.');
        return;
    }

    // .approvalListPopup의 .lineWrap에만 결재선 데이터를 추가
    $(targetSelector).append(approvalLineData);
	setApprovalLineEdit(true, targetSelector);
}

/**
 * 결재선 삭제
 */
function deleteApprovalLine(target) {
	$(target).closest('li').remove();
}


/**
 * 이동파일 조회 팝업 오픈
 */
function openMoveFileListSearchPopup() {
    $('.popup.moveFileListSearchPopup').removeClass('hide');
    const searchOption = {
        requestApprovalCode: 'M',
        statusCd: 'PE'
    };
    // clickSelectData를 핸들러 함수로 전달
    fnSearchList(moveFileListTable, '#grid02', searchOption, clickSelectData);
}

/**
 * 검색 버튼 클릭 이벤트
 */
function checkSearch() {
    // 검색 출처 확인
    if($('.popup.moveFileListSearchPopup:visible').length > 0) {
		const searchOption = {
			requestApprovalCode: 'M',
			statusCd: 'PE'
		};
		fnSearchList(moveFileListTable, '#grid02', searchOption, clickSelectData);
    }
	else {
		const searchOption = {
			startDt: $('#startDt').val(),
			endDt: $('#endDt').val(),
			requestApprovalCode: $('#approvalCode').val(),
			statusCd: $('#statusCd').val(),
			resultCd: $('#resultCd').val(),
			searchType: $('#searchType').val(),
			searchWord: $('#searchWord').val()
		};
		fnSearchList(requestTable, '#grid01', searchOption, clickViewData);
	}
}

/**
 * moveFileListSearchPopup 에서 사용하는 핸들러 함수
 * 테이블 데이터 선택 시 호출된다
 */
function clickSelectData(targetTable) {
    var table = $(targetTable).DataTable();
    
    // 클릭 이벤트 핸들러 등록
    $(targetTable + ' tbody').on('click', 'tr', function() {
        var data = table.row(this).data();
        if (data) {
            console.log('선택된 데이터:', data);

			// 데이터를 폼에 적용한다
			$('#requestTitle').val(data.title);
			$('#requestReasonM').val(data.reason || '');
			// 신철날자 업데이트
			$('#requestDate').text(data.createdDate);
			// 신청자 업데이트
			$('#requestUserNm').text(data.userNm);
			$('#requestUserId').val(data.userId);
			// 신청 번호 업데이트
			$('#requestCode').text(data.requestCode);

			// 등록 파일을 추가하기 위해 fileGroupId로 파일 목록을 조회한다
			loadFileList(data.fileGroupId);

			// 결재선 불러오기
			loadApprovalLine(data.requestCode);

            // 팝업 닫기
            $('.moveFileListSearchPopup').addClass('hide');
        }
    });
}

/**
 * 파일 목록 조회
 */
function loadFileList(fileGroupId) {

	$.ajax({
		url: '/primx/fun/rqt/createRequest/loadFileList.do',
		method: 'POST',
		data: { fileGroupId: fileGroupId },
		success: function(response) {
			console.log('파일 목록:', response);
			// 파일 목록을 초기화
			$('.approvalFormTypeM .upFileList').empty();

			var fileList = '';
			for(var i = 0; i < response.data.length; i++) {
				fileList += '<div>' + response.data[i].fileName + '</div>';
			}

			// 파일이 없는경우 추가하지 않는다
			if(fileList.length > 0) {
				$('.approvalFormTypeM .upFileList').append(fileList);
			}

			// <div class="upFileList" id="viewFiles">
			// <div>PRIMX  신청 및 결재  신청 (6).csv</div>
			// <div>시스템 현황 2024-11-29.csv</div>
			// <div>PRIMX  신청 및 결재  신청 (5).csv</div>
			// </div>

		},
		error: function(xhr, status, error) {
			console.error('파일 목록 조회 오류:', error);
		}
	});
}

/**
 * 결재선 불러오기
 */
function loadApprovalLine(requestCode) {
	// 불러오는 경우 기존 결재선은 초기화
	var targetSelector = '.popup.createRequestPopup.innerVerPop .pop_cont .lineWrap';
	
	$(targetSelector).find('li').remove();
	initApprovalLine(targetSelector, null);

	console.log('requestCode: [' + requestCode + ']');
	$.ajax({
		url: '/primx/fun/rqt/createRequest/loadApprovalLine.do',
		method: 'POST',
		data: { requestCode: requestCode },
		success: function(response) {
			console.log('결재선 데이터:', response);
			// 오더 번호 순서대로 결재선 데이터를 추가한다
			for(var i = 0; i < response.data.length; i++) {
				//var approvalLineData = '<li class="otherDiv editLi" draggable="true">';
				var approvalLineData = genApprHtml(response.data[i].emplyrId, response.data[i].deptId, response.data[i].deptName, response.data[i].userNm, response.data[i].userRole, '-', '');
				$(targetSelector).append(approvalLineData);
			}
		},
		error: function(xhr, status, error) {
			console.error('결재선 불러오기 오류:', error);
		}
	});
}

function initSearch(popupName) {
	$('#popupName').val(popupName);
	$('#approvalCodeLine').removeClass('hide');
	
	if(popupName == 'moveFileListSearchPopup') {
		$('#approvalCodeLine').addClass('hide');
	}
}

// 검색
function searchCreateRequest() {

	var popupName = $('#popupName').val();
	
	var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
	var searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
	var searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
	var searchApprovalCode = $(".searchAreaPopup .search_wrap select[name='approvalCode']");
	var searchStatusCd = $(".searchAreaPopup .search_wrap select[name='statusCd']");
	var searchResultCd = $(".searchAreaPopup .search_wrap select[name='resultCd']");
	var searchSearchType = $(".searchAreaPopup .search_wrap select[name='searchType']");
	var searchSearchWord = $(".searchAreaPopup .search_wrap input[name='searchWord']");

    if(popupName == 'searchAreaPopup') {
		
		if(searchDateType.val() != null && searchDateType.val() != "") {
			if(!searchFieldValid('date', searchStartDate.val(), searchEndDate.val(), searchDateType.val())) {
				return;
			}
			var filterStr = searchStartDate.val() +' ~ '+ searchEndDate.val();
			addFilterItem(".createRequest .searchBox", "기간", filterStr, popupName);
		}
		else {
			resetSearchValue(popupName, "기간");
		}

		if(searchApprovalCode.val() != null && searchApprovalCode.val() != "") {
			// 선택된 값의 라벨을 추가
			addFilterItem(".createRequest .searchBox", "신청 분류", searchApprovalCode.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "신청 분류");
		}

		if(searchStatusCd.val() != null && searchStatusCd.val() != "") {
			// 선택된 값의 라벨을 추가
			addFilterItem(".createRequest .searchBox", "상태", searchStatusCd.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "상태");
		}

		if(searchResultCd.val() != null && searchResultCd.val() != "") {
			// 선택된 값의 라벨을 추가
			addFilterItem(".createRequest .searchBox", "결과", searchResultCd.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "결과");
		}

		if(searchSearchType.val() != null && searchSearchType.val() != "" && searchSearchWord.val() != null && searchSearchWord.val() != "") {
			if(!searchFieldValid('condition', searchSearchWord.val(), null, null)) {
				return;
			}
			// 선택된 값의 라벨을 추가
			addFilterItem(".createRequest .searchBox", "조건", searchSearchType.find('option:selected').text() + " [" + searchSearchWord.val() + "]", popupName);
		}
		else {
			resetSearchValue(popupName, "조건");
		}
		fnSearchList(requestTable, '#grid01', null, clickViewData);
		$('.searchAreaPopup').addClass('hide');
	}
	else if(popupName == 'moveFileListSearchPopup') {
		
		if(searchDateType.val() != null && searchDateType.val() != "") {
			if(!searchFieldValid('date', searchStartDate.val(), searchEndDate.val(), searchDateType.val())) {
				return;
			}
			var filterStr = searchStartDate.val() +' ~ '+ searchEndDate.val();
			addFilterItem(".moveFileListSearchPopup .searchBox", "기간", filterStr, popupName);
		}
		else {
			resetSearchValue(popupName, "기간");
		}

		if(searchApprovalCode.val() != null && searchApprovalCode.val() != "") {
			// 선택된 값의 라벨을 추가
			addFilterItem(".moveFileListSearchPopup .searchBox", "신청 분류", searchApprovalCode.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "신청 분류");
		}

		if(searchStatusCd.val() != null && searchStatusCd.val() != "") {
			// 선택된 값의 라벨을 추가
			addFilterItem(".moveFileListSearchPopup .searchBox", "상태", searchStatusCd.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "상태");
		}

		if(searchResultCd.val() != null && searchResultCd.val() != "") {
			// 선택된 값의 라벨을 추가
			addFilterItem(".moveFileListSearchPopup .searchBox", "결과", searchResultCd.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "결과");
		}

		if(searchSearchType.val() != null && searchSearchType.val() != "" && searchSearchWord.val() != null && searchSearchWord.val() != "") {
			if(!searchFieldValid('condition', searchSearchWord.val(), null, null)) {
				return;
			}
			// 선택된 값의 라벨을 추가
			addFilterItem(".moveFileListSearchPopup .searchBox", "조건", searchSearchType.find('option:selected').text() + " [" + searchSearchWord.val() + "]", popupName);
		}
		else {
			resetSearchValue(popupName, "조건");
		}

		
		const searchOption = {
			startDt: $('#startDt').val(),
			endDt: $('#endDt').val(),
			requestApprovalCode: 'M',
			statusCd: 'PE',
			resultCd: $('#resultCd').val(),
			searchType: $('#searchType').val(),
			searchWord: $('#searchWord').val()
		};

		fnSearchList(moveFileListTable, '#grid02', searchOption, clickViewData);
		$('.searchAreaPopup').addClass('hide');
	}
	else {
		console.log('searchCreateRequest: no match popupName: ', popupName);
	}

}
 
function removeFilter(element, popupName) {

	// 선택된 필터의 filterTitle을 찾아서 초기화함수 호출
	var filterTitle = $(element).closest('.filterItem').find('.filterTitle').text();
	resetSearchValue(popupName, filterTitle);
	$(element).closest('.filterItem').remove(); // 해당 필터 아이템 제거

    if(popupName == 'searchAreaPopup') {
		fnSearchList(requestTable, '#grid01', null, clickViewData);
    }
}

function resetSearchValue(popupName, filterTitle) {
    console.log('resetSearchValue: ', filterTitle);

    if(popupName == 'searchAreaPopup') {
		var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
		var searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
		var searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
		var searchApprovalCode = $(".searchAreaPopup .search_wrap select[name='approvalCode']");
		var searchStatusCd = $(".searchAreaPopup .search_wrap select[name='statusCd']");
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
		else if(filterTitle == "상태") {
			searchStatusCd.val(searchStatusCd.find('option[value=""]').val());
		}	
		else if(filterTitle == "결과") {
			searchResultCd.val(searchResultCd.find('option[value=""]').val());
		}
		else if(filterTitle == "조건") {
			searchSearchType.val(searchSearchType.find('option[value="all"]').val());
			searchSearchWord.val('');
		}

	}
	else if(popupName == 'moveFileListSearchPopup') {
		var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
		var searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
		var searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
		var searchApprovalCode = $(".searchAreaPopup .search_wrap select[name='approvalCode']");
		var searchStatusCd = $(".searchAreaPopup .search_wrap select[name='statusCd']");
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
		else if(filterTitle == "상태") {
			searchStatusCd.val(searchStatusCd.find('option[value=""]').val());
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

