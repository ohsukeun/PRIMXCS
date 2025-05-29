/*function loadMainPage() {
	//알림에서 페이지 접근시 왼쪽메뉴 셀렉트 되도록
	var firstDepth = $(".firstDepthMenu .approval");
	$(".left li").removeClass("select");
	$(firstDepth).closest("li").addClass("select open");
	$(".createRequestLi").addClass("select");
}
*/
function passwordEyes() {
	/* 로그인창 비밀번호 보이기 */
	$('.pwViewBtn').on('click', function() {
		const $input = $(this).siblings('input.pwInput'); // 클릭한 버튼의 형제 요소인 input을 선택
		$input.toggleClass('on');
		if ($input.hasClass('on')) {
			$input.attr('type', "text");
		} else {
			$input.attr('type', 'password');
		}
	});
} 
// 내 정보 로드, Promise를 반환하도록 수정
function loadMyInfo() {
	console.log('loadMyInfo 호출');
	return new Promise((resolve, reject) => {
		$.ajax({
			url: '/primx/fun/usr/myInfo.do',
			method: 'GET',
			success: function(response) {
				console.log('response in loadMyInfo:', response);
				if (response && response.data) {
					console.log("response:", response.data.deptName);
					resolve(response.data);
				} else {
					reject('데이터가 없습니다.');
				}
			},
			error: function(error) {
				reject(error);
			}
		});
	});
}

function checkUserId(emplyrId) {

	if (!emplyrId) {
		alert('사용자 ID를 입력해주세요.');
		return;
	}

	$.ajax({
		url: '/primx/fun/cmm/checkUserId.do',
		type: 'POST',
		data: { emplyrId: emplyrId },
		success: function(response) {
			if (response.available) {
				alert('사용 가능한 ID입니다.');
				$('input[name="idCheckFlag"]').val('Y');
				$('input[name="checkedUserId"]').val(emplyrId);
			} else {
				alert('이미 존재하는 ID입니다.');
				$('input[name="idCheckFlag"]').val('N');
			}
		},
		error: function() {
			alert('ID 체크 중 오류가 발생했습니다.');
		}
	});
}

function checkMail(mail) {
	console.log('checkMail 호출');

	if (!mail) {
		alert('이메일을 입력해주세요.');
		return;
	}

	$.ajax({
		url: '/primx/fun/cmm/checkMail.do',
		type: 'POST',
		data: { mail: mail },
		success: function(response) {
			if (response.available) {
				alert('사용 가능한 이메일입니다.');
				$('input[name="mailCheckFlag"]').val('Y');
				$('input[name="checkedMail"]').val(mail);
			} else {
				alert('이미 존재하는 이메일입니다.');
				$('input[name="mailCheckFlag"]').val('N');
			}
		},
		error: function() {
			alert('이메일 체크 중 오류가 발생했습니다.');
		}
	});
}

$(document).ready(function() {
	// 타임아웃 카운트다운 설정
	let totalTime = 10 * 60;
	const initialTime = totalTime; // 초기 시간 저장
	const timeOutArea = $(".timeOutArea"); // timeOutArea 선택

	// 동작주기 셀렉트박스 초기 상태 설정 및 이벤트 처리
	const $operationCycleSelect = $('#operationCycleBox select');
	const $weeklyBox = $('#weeklyBox');
	const $monthlyBox = $('#monthlyBox');
	
	// 초기 상태 설정
	function setOperationCycleState() {
		const selectedValue = $operationCycleSelect.val();
		
		if (selectedValue === 'daily') {
			$weeklyBox.addClass('hide');
			$monthlyBox.addClass('hide');
		} else if (selectedValue === 'weekly') {
			$weeklyBox.removeClass('hide');
			$monthlyBox.addClass('hide');
		} else if (selectedValue === 'monthly') {
			$weeklyBox.addClass('hide');
			$monthlyBox.removeClass('hide');
		}
	}

	// 초기 상태 설정 실행
	setOperationCycleState();

	// 변경 이벤트 핸들러 등록
	$operationCycleSelect.on('change', setOperationCycleState);

	function updateTimer() {
		let minutes = Math.floor(totalTime / 60);
		let seconds = totalTime % 60;

		// 다국어 번역된 문장 가져오기 (템플릿 형태)
		let logoutMessage = getTranslation("session.logoutTimer"); 

		// 동적으로 시간 값 삽입
		logoutMessage = logoutMessage
			.replace("{minutes}", minutes)
			.replace("{seconds}", seconds < 10 ? '0' + seconds : seconds);

		// UI 업데이트
		timeOutArea.html(logoutMessage); // timeOutArea 업데이트

		// 타이머가 0이 되면 로그아웃 처리
		if (totalTime <= 0) {
			clearInterval(timerInterval);
			// 로그아웃 처리 전에 서버에 세션 만료 알림
			$.ajax({
				url: '/primx/fun/cmm/sessionExpired.do',
				type: 'POST',
				async: false, // 동기 처리로 변경
				success: function() {
					// 로그아웃 함수 호출
					logout();
					alert("세션이 만료되었습니다. 로그아웃됩니다.");
					// 로그인 페이지로 강제 이동
					window.location.href = '/uat/uia/egovLoginUsr.do';
				},
				error: function() {
					alert("세션이 만료되었습니다. 로그아웃됩니다.");
					// 실패시에도 강제로 로그인 페이지로 이동
					window.location.href = '/uat/uia/egovLoginUsr.do';
				}
			});
		}

		totalTime--;
	}

	// 1초마다 updateTimer 함수 호출
	const timerInterval = setInterval(updateTimer, 1000);

	// 이벤트 리스너 등록 (a, button, td 태그 클릭 시 타이머 초기화)
	$(document).on("click", "a", resetTimer);
	$(document).on("click", "button", resetTimer);
	$(document).on("click", "td", resetTimer);
	$(document).on("click", "input", resetTimer);

	// input에 텍스트 입력하고 있을때 타이머 초기화
	$(document).on("input", "input", resetTimer);
	$(document).on("change", "select", resetTimer);
	$(document).on("change", "textarea", resetTimer);
	$(document).on("change", "input[type='checkbox']", resetTimer);
	$(document).on("change", "input[type='radio']", resetTimer);
	$(document).on("change", "input[type='file']", resetTimer);

	// 이벤트가 발생하면 타이머 초기화
	function resetTimer() {
		totalTime = initialTime;
	}

	// 기간 선택 이벤트 핸들러 함수
	function handlePeriodSelect($select, $startDatePicker, $endDatePicker) {
		const selectedValue = $select.val();
		
		if (selectedValue === '') {
			// 기간 지정 선택 시 datepicker 활성화
			$startDatePicker.prop('disabled', false);
			$endDatePicker.prop('disabled', false);
			// 날짜 초기화
			$startDatePicker.val('');
			$endDatePicker.val('');
		} else {
			// 다른 옵션 선택 시 datepicker 비활성화
			$startDatePicker.prop('disabled', true);
			$endDatePicker.prop('disabled', true);
			
			// 선택된 개월 수에 따라 자동으로 날짜 설정
			const endDate = new Date();
			const startDate = new Date();
			startDate.setMonth(startDate.getMonth() - parseInt(selectedValue));
			
			// YYYY-MM-DD 형식으로 변환
			$startDatePicker.val('');
			$endDatePicker.val('');
		}
	}

	// 첫 번째 행의 기간 선택 이벤트 핸들러
	$('#periodSelect').off('change').on('change', function() {
		handlePeriodSelect(
			$(this),
			$('#startDatePicker'),
			$('#endDatePicker')
		);
	});

	// 두 번째 행의 기간 선택 이벤트 핸들러
	$('#periodSelect2').off('change').on('change', function() {
		handlePeriodSelect(
			$(this),
			$('#startDatePicker2'),
			$('#endDatePicker2')
		);
	});

	// 초기 상태 설정
	$('#periodSelect').trigger('change');
	$('#periodSelect2').trigger('change');
});

/**
 * 롤권한 정보 가져오기 (Promise를 반환하도록 수정)
 * @param {*} searchType // 검색 타입(0 하위권한 미포함, 1 하위권한 포함, 2 전체권한)
 */
function loadRoleInfo(searchType) {
	return new Promise((resolve, reject) => {
		$.ajax({
			url: '/primx/fun/cmm/roleInfo.do',
			data: {
				searchType: searchType
			},
			method: 'GET',
			success: function(data) {
				console.log(data);
				resolve(data); // 데이터 반환
			},
			error: function(err) {
				reject('ajax call error');
			}
		});
	});
}


// 회사 정보 로드
function loadCompanyInfo(targetSelectBox) {
	console.log("targetSelectBox: ", targetSelectBox);
	$.ajax({
		url: '/primx/fun/cmm/companyInfo.do',
		method: 'GET',
		success: function(response) {
			console.log(response);
			var companyInfo = response.data;
			// 앞에 클래스를 추가하여 선택된 회사 표시
			var selectBox = $(targetSelectBox);
			selectBox.empty();

			for (var i = 0; i < companyInfo.length; i++) {
				selectBox.append('<option value="' + companyInfo[i].companyId + '">' + companyInfo[i].companyName + '</option>');
			}
		},
		error: function(xhr, status, error) {
			console.error("부서 트리 로드 실패:", {
				status: status,
				error: error,
				response: xhr.responseText
			});
			alert("부서 트리를 불러오는 중 오류가 발생했습니다.");
		}
	});
}

// 부서 선택 팝업 열기
function openDepartSelectPopup(searchKeyword, parentId) {
	// 먼저 팝업을 보이게 함
	$(".departSelectPopup").removeClass("hide");

	console.log('searchKeyword: ', searchKeyword);
	console.log('parentId: ', parentId);

	$.ajax({
		url: '/primx/fun/dpt/tree.do',
		type: 'POST',
		data: {
			searchKeyword: searchKeyword || '',
			parentId: parentId || '',
			includeSubDept: true
		},
		success: function(response) {
			if (response && response.data) {
				const container = document.querySelector('.departSelectPopup .departTreeMap');
				if (!container) {
					console.error('부서 트리를 표시할 컨테이너를 찾을 수 없습니다.');
					return;
				}

				// 두 번째 버전의 renderDepartSelectTree 사용
				renderDepartSelectTree(response.data, container, function(deptName, deptId, companyId) {
					$("input[name='deptName']").val(deptName);
					$("input[name='deptId']").val(deptId);
					$("input[name='companyId']").val(companyId);
					$("select[name='companyId']").val(companyId);
					$("input[name='isDeptIdValid']").val(true);
					closeDepartSelectPopup();
				});
			}
		},
		error: function(xhr, status, error) {
			console.error("부서 트리 로드 실패:", error);
		}
	});
}

// 부서 선택 팝업 닫기
function closeDepartSelectPopup() {
	$(".departSelectPopup").addClass("hide");
}

/**
 * 부서 트리 로드
 * @param {string} searchKeyword - 검색 키워드
 * @param {string} parentId - 부모 ID
 * @returns {Promise} - 트리 데이터를 반환하는 Promise
 */
function loadDeptTree(searchKeyword, parentId) {
	return new Promise((resolve, reject) => {
		$.ajax({
			url: '/primx/fun/dpt/tree.do',
			type: 'POST',
			data: {
				searchKeyword: searchKeyword || '',
				parentId: parentId || ''
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

/**
 * 부서 트리 로드 및 렌더링
 * @param {string} containerClass - 트리를 렌더링할 컨테이너의 클래스 이름
 * @param {string} containerId - 트리를 렌더링할 컨테이너의 ID
 * @param {string} searchKeyword - 검색 키워드
 * @param {string} parentId - 부모 ID
 * @param {Function} callback(deptName, deptId, companyId) - 선택된 부서 정보를 처리할 콜백 함수
 */
function loadAndRenderDeptTree(containerClass, containerId, searchKeyword, parentId, callback) {
	$.ajax({
		url: '/primx/fun/dpt/tree.do',
		type: 'POST',
		data: {
			searchKeyword: searchKeyword || '',
			parentId: parentId || '',
			includeSubDept: $('#includeSubDept').is(':checked') ? true : false
		},
		success: function(response) {
			if (response && response.data) {
				// console.log('부서 트리 데이터1111:', response.data);

				// ID로 선택
				const containerById = document.getElementById(containerId);
				// 클래스 이름으로 선택
				const containerByClass = document.querySelector(`.${containerClass}`);

				// ID가 우선적으로 사용되도록 설정
				const container = containerById || containerByClass;

				console.log('container:', container);
				if (container) {
					renderDepartSelectTree(response.data, container, callback);
				} else {
					console.error('컨테이너를 찾을 수 없습니다.');
				}
			}
		},
		error: function(xhr, status, error) {
			console.error("부서 트리 로드 실패:", error);
		}
	});
}

/**
 * 부서 선택 트리 렌더링
 * @param {Array} treeData - 트리 데이터
 * @param {HTMLElement} container - 트리를 렌더링할 컨테이너
 * @param {Function} callback - 선택된 부서 정보를 처리할 콜백 함수
 */
function renderDepartSelectTree(treeData, container, callback) {
	const tree = new CustomTree(container, {
		expandAll: true,
		onSelect: function(deptName, deptId, companyId) {
			console.log('선택된 부서 정보:', {
				deptName: deptName,
				deptId: deptId,
				companyId: companyId
			});

			// 콜백 함수 호출
			if (callback && typeof callback === 'function') {
				callback(deptName, deptId, companyId);
			}
		}
	});

	tree.render(treeData);
}

/**
 * 사용자 검색 팝업 오픈
 * @param {Object} dataTable - 데이터 테이블 객체
 * @param {string} tableSelector - 초기화할 데이터 테이블의 선택자
 * @param {Object} searchConditions - 검색 조건을 포함하는 객체
 * @param {Array} columns - 사용할 컬럼을 정의하는 배열
 * @param {Object} additionalOptions - 추가 옵션 (columnDefs, buttons, select 등)
 * @returns {Object} - 초기화된 DataTable 객체
 */
function loadDataTable(dataTable, tableSelector, searchConditions, columns, additionalOptions = {}) {
	// 기본 설정
	let tableOptions = {
		ajax: {
			url: '/primx/fun/cmm/getDeptUserIdList.do',
			type: 'POST',
			data: function(d) {
				// 검색 조건 추가
				for (const key in searchConditions) {
					d[key] = searchConditions[key];
				}
				// 페이지 번호와 페이지 크기 추가
				d.page = d.start / d.length + 1;
				d.pageSize = d.length;
			}
		},
		processing: true,
		serverSide: true,
		columns: columns
	};

	// 추가 옵션 병합
	if (additionalOptions) {
		for (const key in additionalOptions) {
			if (additionalOptions[key] !== null && additionalOptions[key] !== undefined) {
				tableOptions[key] = additionalOptions[key];
			}
		}
	}

	// 데이터테이블 초기화 및 반환
	const initializedTable = initDataTables(tableSelector, tableOptions);

	// 클릭 이벤트 핸들러 설정
	$(tableSelector + ' tbody').off('click');
	$(tableSelector + ' tbody').on('click', 'td:not(:first-child)', function() {
		const data = initializedTable.row(this).data();
		if (data) {
			clickEventHandler(data);
		}
	});

	return initializedTable; // 초기화된 테이블 반환
}


/**
 * 필터 아이템 추가
 * @param {string} className - 필터 아이템을 추가할 요소의 클래스 이름
 * @param {string} filterTitle - 필터 아이템에 표시할 제목
 * @param {string} keyword - 필터 아이템에 표시할 검색어
 * @param {String} popupName - 필터 아이템 삭제 시 팝업 이름
 * @param {String} customRemoveFilter - 필터 아이템 삭제 시 사용할 함수 이름
 */
function addFilterItem(className, filterTitle, keyword, popupName, customRemoveFilter) {

	// className 클래스에 동일한 filterTitle의 아이템이 있는지 검사한다
	var deleteFilterItem = $(className).find('.filterTitle:contains("' + filterTitle + '")');
	if (deleteFilterItem.length > 0) {
		console.log('동일한 필터 아이템이 있습니다. deleteFilterItem:', deleteFilterItem);
		// 해당 필터 아이템의 상위 클래스를 지운다
		$(deleteFilterItem).closest('.filterItem').remove();
	}

	var removeFilter = 'removeFilter';
	if (customRemoveFilter != null && customRemoveFilter != "" && customRemoveFilter != undefined) {
		removeFilter = customRemoveFilter;
	}

	var filterItem = '<p class="filterItem">';
	filterItem += '<span class="filterTitle">' + filterTitle + '</span>';
	filterItem += '<span> : </span>';
	filterItem += '<span class="filterSelect">' + keyword + '</span>';
	filterItem += '<span class="delBtn" onclick="' + removeFilter + '(this, \'' + popupName + '\');"></span>';
	filterItem += '</p>';
	$(className).append(filterItem);
}

/**
 * 검색조건 검증
 */
function searchFieldValid(type, value1, value2, value3) {
	if(type == 'date') {
		if(value3 == "") {
			return true;
		}
		var startDt = $.trim(value1);
		var endDt = $.trim(value2);
		if(startDt == '') {
			alert('검색 시작일을 선택하세요.');
			return false;
		}
		if(endDt == '') {
			alert('검색 종료일을 선택하세요.');
			return false;
		}
		var fromDt = new Date(startDt);
		var toDt = new Date(endDt);
		if (fromDt.getTime() > toDt.getTime()) {
			alert('검색 종료일은 시작일보다 이후여야 합니다.');
			return false;
		}
	}
	else if(type == 'condition') {
		var searchWord = $.trim(value1);
		if (searchWord == '') {
			alert('검색어를 입력하세요.');
			return false;
		}
	}
	return true;
}

function initUserEventListeners() {

	console.log('initUserEventListeners');

	// datepicker 초기화
    $('.datepicker').datepicker({
        dateFormat: 'yy-mm-dd',
        onSelect: function(dateText, inst) {
            $(this).datepicker('hide');
        }
    });

    // 사용기간 select box 변경 이벤트 처리
    $('.useDate .selectBox select').on('change', function() {
        var selectedValue = $(this).val();
        var $expiredDateInput = $(this).closest('.useDate').find('#userRegistrationExpiredDate');
        
        if (selectedValue === '0') { // 무기한
            $expiredDateInput
                .val('9999-12-31')
                .attr('disabled', true)  // disabled 속성 추가
        } else { // 기간 지정
            $expiredDateInput
                .val('')
                .attr('disabled', false)  // disabled 속성 제거
        }
    });

    // 페이지 로드 시 초기 상태 설정
    $('.useDate .selectBox select').trigger('change');

}


/**
 * 년-월-일 설정
 * @param {string} dateStr - 날짜문자열
 * @returns {string} - 년-월-일 문자열
 */

function setYYMMDD(dateStr) {

	if (!dateStr) return '';
	const dateParts = dateStr.split('-');
	const year = dateParts[0];
	const month = dateParts[1];
	const day = dateParts[2].substring(0, 2);

	return `${year}-${month}-${day}`;
}



// 통합 통합 통합
// 통합 통합 통합
// 통합 통합 통합
// 통합 통합 통합
// 통합 통합 통합


function setFormName(requestApprovalCode) {
	var formName = '';
	if (requestApprovalCode == 'M') {
		formName = '이동파일 생성 신청';
	} else if (requestApprovalCode == 'C') {
		formName = '계정 연장 신청';
	} else if (requestApprovalCode == 'A') {
		formName = '사용자 등록 신청';
	}
	return formName;
}

/**
 * 신청 저장
 */
async function saveRequest() {
	// 서식 설정 가져오기
	var requestApprovalCode = $('#requestApprovalCode').val();
	var reason = '';

	var userId = $('#userId').val();
	var idCheckFlag = $('input[name="idCheckFlag"]').val();
	var checkedUserId = $('input[name="checkedUserId"]').val();
	var email = $('#email').val();
	var mailCheckFlag = $('input[name="mailCheckFlag"]').val();
	var checkedMail = $('input[name="checkedMail"]').val();
	
	// 제목 가져오기 및 검증
	var requestTitle = $.trim($('#requestTitle').val());
	if (requestTitle === '') {
		alert('제목을 입력하세요.');
		return;
	}

	if (requestApprovalCode == 'M') {
		reason = $('#requestReasonM').val();
	}
	else if (requestApprovalCode == 'C') {
		reason = $('#requestReasonC').val();
	}
	else if (requestApprovalCode == 'A') {
		// 사용자 id 체크

		// 데이터 검증
		if (!userId) {
			alert('사용자 ID를 입력해주세요.');
			return;
		}
		if (idCheckFlag === 'N') {
			alert('이미 존재하는 ID입니다.');
			return;
		}
		if (checkedUserId !== userId) {
			alert('사용자 ID가 변경되었습니다. 다시 확인해주세요.');
			return;
		}
		if(!email) {
			alert('이메일을 입력해주세요.');
			return;
		}
		if (mailCheckFlag === 'N') {
			alert('이미 존재하는 이메일입니다.');
			return;
		}
		if (checkedMail !== email) {
			alert('이메일이 변경되었습니다. 다시 확인해주세요.');
			return;
		}
		reason = $('#requestReasonA').val();
	}
	
	// 사유 입력 검증
	reason = $.trim(reason);
	if (reason === '') {
		alert('사유를 입력하세요.');
		return;
	}

	// 결재선에 등록된 리스트 정보 가져오기
	var approvalLines = [];
	$('.popup.createRequestPopup.innerVerPop .pop_cont .lineWrap li').each(function() {
		if (!$(this).hasClass('startLine') && !$(this).hasClass('addLineDiv')) {
			var approvalLine = {
				orderNumber: $(this).index() - 1,
				userId: $(this).find('input[name="approvalUserId"]').val(),
				userRole: $(this).find('input[name="approvalUserRole"]').val()
			};
			approvalLines.push(approvalLine);
		}
	});

	// console.log('requestReasonA: ', $('#requestReasonA').val());
	// console.log('requestReasonM: ', $('#requestReasonM').val());
	// console.log('requestReasonC: ', $('#requestReasonC').val());
	// console.log('reason: ', reason);

	// 결재선 최소 2개 이상 체크
	if (approvalLines.length < 2) {
	    alert('결재선을 등록해 주세요.');
	    return;
	}

	var approvalTarget = await setApprovalTarget(approvalLines[0].userRole, requestApprovalCode);

	console.log('approvalTarget: ', approvalTarget);
	
	if(approvalTarget == 'A') {
		// 마지막 결재자가 
		if(!approvalLines[approvalLines.length-1].userRole.includes('ADMIN')) {
			alert('최종 결재자는 관리자 여야 합니다.');
			return;
		}
	}
	else if(approvalTarget == 'U') {
		// 마지막 결재자가 
		if(!approvalLines[approvalLines.length-1].userRole.includes('USER')) {
			alert('최종 결재자는 사용자 여야 합니다.');
			return;
		}
	}

	
	// 신청자 정보 가져오기
	var regiestUserInfo = {
		requestDate: $('#requestDate').text(),
		requestUserId: $('#requestUserId').val(),
		requestTitle: $('#requestTitle').val(),
		requestReason: reason
	};

	// 만료일자 설정

	var usePeriodEnd = $('.createRequestPopup #usePeriodEnd').val();
	var expiredDate = $('.createRequestPopup #expiredDate').val();
	// console.log('expiredDate: ', expiredDate);
	// console.log('usePeriodEnd: ', usePeriodEnd);

	 if (requestApprovalCode == 'C') {
        expiredDate = $('.createRequestPopup #accountExtensionPeriodEnd').val();
        if (!expiredDate) {
            alert('만료일자를 입력해주세요.');
            return;
        }
    }
    else if (requestApprovalCode == 'A') {
        var usePeriod = $('.approvalFormTypeA #userRegistrationPeriod').val();
        if (usePeriod === '0') {
            expiredDate = '9999-12-31';
        } else {
            expiredDate = $('.approvalFormTypeA #userRegistrationExpiredDate').val();
            if (!expiredDate) {
                alert('만료일자를 입력해주세요.');
                return;
            }
        }
    }

	var sendData = {
		requestCode: $('#requestCode').text(),
		requestApprovalCode: requestApprovalCode,

		approvalLines: approvalLines,
		regiestUserInfo: regiestUserInfo,
		usePeriod: $('#usePeriod').val(),
		userId: $('#userId').val(),
		userName: $('#userName').val(),
		email: $('#email').val(),
		companyId: $('#companyId').val(),

		deptId: $('#deptId').val(),
		userRole: $('#userRole').val(),
		expiredDate: expiredDate
	};

	console.log('전송할 데이터:', sendData);

	$.ajax({
		url: '/primx/fun/rqt/createRequest/save.do',
		method: 'POST',
		contentType: 'application/json',
		data: JSON.stringify(sendData),
		success: function(response) {
			console.log('서버 응답:', response);
			if (response.status === 'success') {
				alert('저장되었습니다.');
				$('.popup.createRequestPopup.innerVerPop').addClass('hide');
				listReload();
			} else {
				alert(response.message || '저장 중 오류가 발생했습니다.');
			}
		},
		error: function(xhr, status, error) {
			console.error('에러 상세:', {
				status: status,
				error: error,
				response: xhr.responseText
			});
			alert('서버 통신 중 오류가 발생했습니다.');
		}
	});
}

function applyApprovalLine() {
	console.log('applyApprovalLine - callPageClass: ', callPageClass);

	// 타깃
	var targetSelector = '.popup' + callPageClass + ' .pop_cont .lineWrap';
	$(targetSelector).find('li').remove();
	// 적용 데이터
	var approvalLineData = $('.popup.approvalListPopup.innerVerPop .pop_cont .lineWrap').find('li');

	// 결재선 데이터 추가
	initApprovalLine(targetSelector, approvalLineData.slice(1));

	// 결재선 수정 여부 설정
	setApprovalLineEdit(false, targetSelector);

	$(approvalLineData).find('li').remove();


	// 팝업 닫기
	$('.popup.approvalListPopup.innerVerPop').addClass('hide');

	// .createRequestPopup은 결재선 수정이 되지 않도록
	//$('.popup' + callPageClass + ' .otherDiv').removeClass('editLi').attr('draggable', false);
}

function closeApprovalListPopup() {
	var approvalLineData = $('.popup.approvalListPopup.innerVerPop .pop_cont .lineWrap').find('li');
	$(approvalLineData).find('li').remove();
	// 현재 있는 결재선을 전달한다
	$('.popup.approvalListPopup.innerVerPop').addClass('hide');
}

/**
 * 결재선 초기화

 * @param targetSelector 타깃 선택자

 * @param approvalLineData 결재선 데이터
 */
function initApprovalLine(targetSelector, approvalLineData) {
	var defaultLine = '<li class="startLine" data-i18n="createRequest.createRequestPopup.approvalLine">결<br>재<br>선</li>';
	$(targetSelector).append(defaultLine);
	$(targetSelector).append(approvalLineData);
	$(targetSelector).append(createApprovalAddBtn());
	// 🌟 추가된 부분: 동적으로 생성된 HTML 요소에 다국어 적용
	applyTranslations();
}

/**
 * 결재 추가버튼 생성 함수
 */
function createApprovalAddBtn() {
	var addLineDiv = '<li draggable="true" class="addLineDiv registrationApprovalBtn innerVerBtn">';
	addLineDiv += '<button class="registrationApprovalBtn innerVerBtn" type="button" onclick="addApprovalLine();">+</button>';
	addLineDiv += '</li>';
	return addLineDiv;
}

/**
 * 결재선 정보 html 생성
 * @param {string} userId - 결재자 ID
 * @param {string} deptId - 결재자 부서 ID
 * @param {string} deptNm - 결재자 부서명
 * @param {string} userNm - 결재자 이름
 * @param {string} result - 결재 결과
 * @param {string} date - 결재 일자
 */
function genApprHtml(userId, deptId, deptNm, userNm, userRole, result, date) {
	console.log('genApprHtml: ', userId, deptId, deptNm, userNm, userRole, result, date);
	var html = '<li class="otherDiv " draggable="false">'
		+ '<button class="deleteBtn hide" onclick="deleteApprovalLine(this);"></button>'
		+ '<span class="tit">' + deptNm + '</span>'
		+ '<div class="info" draggable="false">'
		+ '<p class="name" draggable="false">' + userNm + '</p>'
		+ '<input class="hide" name="approvalUserId" value="' + userId + '">'
		+ '<input class="hide" name="approvalDeptId" value="' + deptId + '">'
		+ '<input class="hide" name="approvalUserRole" value="' + userRole + '">'
		+ '<p class="result " draggable="false">' + result + '</p>'
		+ '<p class="date " draggable="false">' + date + '</p>'
		+ '</div>'
		+ '</li>';
	// console.log('html: ', html);
	return html;
}


/**
 * 결재선 수정 여부 설정
 * @param {boolean} flag - 수정 여부
 * @param {string} targetSelector - 타깃 선택자
 */
function setApprovalLineEdit(flag, targetSelector) {
	if (flag) {
		$(targetSelector).find('li').addClass('editLi').attr('draggable', true);
		$(targetSelector).find('li').find('.deleteBtn').removeClass('hide');
	}
	else {
		$(targetSelector).find('li').removeClass('editLi').attr('draggable', false);
		$(targetSelector).find('li').find('.deleteBtn').addClass('hide');
	}
	// 첫번째 결재선과 두번째 결재선은 수정 불가능하도록 설정
	$(targetSelector).find('li').eq(0).removeClass('editLi').attr('draggable', false);
	$(targetSelector).find('li').eq(1).removeClass('editLi').attr('draggable', false);
	$(targetSelector).find('li').eq(1).find('.deleteBtn').addClass('hide');
}
/**
 * 신청 팝업 초기화
 */
function clearCreateRequestPopup(myInfo) {
	// 결재선 초기화, 기본값으로 초기화
	// 여기서 내 정보에 개한 결재선 추가
	var approvalLineData = genApprHtml(myInfo.emplyrId, myInfo.deptId, myInfo.deptName, myInfo.userName, myInfo.userRole, '-', '');

	// 추가 버튼 생성하여 파라미터로 전달
	var addLineDiv = approvalLineData;

	// 결재선 초기화
	var createRequestPopup = $('.popup.createRequestPopup.innerVerPop .pop_cont .lineWrap');
	// 가지고있는 결재선 삭제
	$(createRequestPopup).find('li').remove();

	initApprovalLine(createRequestPopup, addLineDiv);

	$('#requestApprovalCode').prop('disabled', false);
	
	$('#moveFielViewBtn').addClass('hide');
	
	// 타이틀 초기화
	$('#requestTitle').val('');

	// 신청번호 초기화
	$('#requestCode').text('자동채번');

	// 사유 초기화
	$('#requestReasonM').val('');
	$('#requestReasonC').val('');
	$('#requestReasonA').val('');

	// 등록 파일 초기화
	$('#viewFiles').empty();

	// 서식 : 계정 연장 신청
	// 사용기간 초기화
	$('#usePeriod').val(0);
	$('#usePeriodStart').val('');
	$('#usePeriodEnd').val('');

	// 서식 : 사용자 등록 신청
	// 사용자 정보 초기화
	$('#userId').val('');
	$('#userName').val('');
	$('#email').val('');
	$('#companyId').val('');
	$('#deptName').val('');
	$('#deptId').val('');
	$('#isDeptIdValid').val(false);
	$('#userRole').val('');
	// 사용기한 초기화
	$('.approvalFormTypeA .useDate .selectBox select').val(0);
	$('.approvalFormTypeA .useDate .datepicker').val('');
}

/**
 * 결재선 지정 팝업 오픈
 */
function addApprovalLine() {
	console.log('callPageClass: ', callPageClass);
	var approvalLineData = $('.popup.approvalListPopup.innerVerPop .pop_cont .lineWrap');
	// 가지고있는 결재선 삭제
	$(approvalLineData).find('li').remove();

	// 기존 결재선 가져오기
	var loadApprovalLineData = $('.popup' + callPageClass + ' .pop_cont .lineWrap').find('li');

	// loadApprovalLineData의 복사본을 생성
	var approvalLineDataCopy = loadApprovalLineData.clone(); // 복사본 생성

	initApprovalLine(approvalLineData, approvalLineDataCopy.slice(1));
	// 결재선 추가 버튼 삭제
	$(approvalLineData).find('li.registrationApprovalBtn').remove();

	// 결재선 수정 여부 설정
	setApprovalLineEdit(true, approvalLineData);

	// deptUserTable 초기화
	deptUserTable = null;

	loadAndRenderDeptTree('departTreeMap', 'deptTreeContainer', '', '', loadUserList);
	$('.popup.approvalListPopup.innerVerPop').removeClass('hide');

	// .approvalListPopup 내의 .otherDiv에만 editLi 클래스 추가
	//$('.approvalListPopup .lineWrap .otherDiv').addClass('editLi').attr('draggable', true);
	
	// 드래그 앤 드롭 초기화 함수 호출
	initializeDragAndDrop('.approvalListPopup .lineWrap');
}

/**
 * 드래그 앤 드롭 초기화
 * @param {string} listSelector - 드래그 앤 드롭이 적용될 리스트의 셀렉터
 */
function initializeDragAndDrop(listSelector) {
	const list = document.querySelector(listSelector);
	let draggedItem = null;

	// 드래그 시작
	list.addEventListener('dragstart', handleDragStart);

	// 드래그 중
	list.addEventListener('dragover', handleDragOver);

	// 드래그 종료
	list.addEventListener('dragend', handleDragEnd);

	// 드래그 시작 이벤트 핸들러
	function handleDragStart(e) {
		draggedItem = e.target;

		// 드래그가 시작되면 불투명하게 처리
		draggedItem.style.opacity = '0.5';
		draggedItem.classList.add('dragging');
	}

	// 드래그 중 위치 변경 (실시간으로 드래그되는 항목이 다른 항목과 위치를 교환)
	function handleDragOver(e) {
		e.preventDefault(); // 기본 동작 방지

		// 드래그 중인 항목이 없으면 리턴
		if (!draggedItem) return;

		const target = e.target.closest('li.otherDiv');  // 다른 항목만 대상으로 지정

		// 드래그 대상이 다른 항목이라면 위치 교환
		if (target && target !== draggedItem) {
			// 첫 번째와 두 번째 결재선은 이동 불가
			const isFirstOrSecond = $(target).index() < 2 || $(draggedItem).index() < 2;
			if (isFirstOrSecond) {
				return; // 이동 불가
			}

			// 목표 항목과 드래그된 항목의 위치 비교하여 교환
			const targetRect = target.getBoundingClientRect();
			const draggedRect = draggedItem.getBoundingClientRect();

			// 마우스 Y좌표를 이용해 드래그된 항목이 목표 항목의 위/아래에 있는지 확인
			if (e.clientY < targetRect.top + targetRect.height / 2) {
				// 목표 항목 앞에 드래그된 항목 삽입
				target.parentNode.insertBefore(draggedItem, target);
			} else {
				// 목표 항목 뒤에 드래그된 항목 삽입
				target.parentNode.insertBefore(draggedItem, target.nextElementSibling);
			}
		}
	}

	// 드래그 종료 후
	function handleDragEnd() {
		// 드래그가 끝나면 스타일 복원
		if (draggedItem) {
			draggedItem.style.opacity = ''; // 불투명도 복원
			draggedItem.classList.remove('dragging');
			draggedItem = null;
		}
	}
}


/**
 * 사용자 검색 팝업 오픈
 */
async function initUserSearchPopup(targetClass) {
	// 전체 권한 불러오기
	var roleData = await loadRoleInfo(2);
	console.log('roleData: ', roleData);

	// searchArea_p_Popup userSearchPopup의 userSelectBox 권한 정보 추가
	$(targetClass + ' #userSelectOption').empty();
	$(targetClass + ' #userSelectOption').append('<option value="">전체</option>');

	for (var i = 0; i < roleData.data.length; i++) {
		console.log('roleData[i]: ', roleData.data[i]);
		$(targetClass + ' #userSelectOption').append('<option value="' + roleData.data[i].authorCode + '">' + roleData.data[i].authorDescription + '</option>');
	}

	$(targetClass).removeClass("hide");
	
	$('#selectedUserName').focus();
	
}

/**
 * 부서 검색 팝업 오픈
 * @param {
 * } searchKeyword 
 */
function initDeptSearchPopup(targetClass) {
	$(targetClass).removeClass("hide");
	$('#commonDeptSearchKeyword').focus();
}


function searchApprovalLine(popupName) {
	if(popupName == "searchArea_p_Popup departSearchPopup") {
		var searchKeyword = $('#commonDeptSearchKeyword');
		if(searchKeyword.val() != null && searchKeyword.val() != "") {
			addFilterItem('.approvalListPopup .departmentArea .searchBox', '부서', searchKeyword.val(), popupName, 'removeApprovalLineFilter');
		}
		else {
			resetApprovalLineSearchValue(popupName, '부서');
		}
		
		loadAndRenderDeptTree('departTreeMap', 'deptTreeContainer', searchKeyword.val(), '', loadUserList);
		$('.popup.departSearchPopup').addClass('hide');
	}
	else if(popupName == "searchArea_p_Popup userSearchPopup") {
		var userSelectOption = $('#commonUserSelectOption');
		var hrSelectOption = $('#commonHrSelectOption');

		if(userSelectOption.val() != null && userSelectOption.val() != "") {
			addFilterItem('.approvalListPopup .userArea .searchBox', '사용자', userSelectOption.find('option:selected').text(), popupName, 'removeApprovalLineFilter');
		}
		else {
			resetApprovalLineSearchValue(popupName, '사용자');
		}

		if(hrSelectOption.val() != null && hrSelectOption.val() != "") {
			addFilterItem('.approvalListPopup .userArea .searchBox', '구분', hrSelectOption.find('option:selected').text(), popupName, 'removeApprovalLineFilter');
		}
		else {
			resetApprovalLineSearchValue(popupName, '구분');
		}

		// console.log('userSelectOption.val(): ', userSelectOption.val());
		// console.log('hrSelectOption.val(): ', hrSelectOption.val());
		// console.log('searchKeyword: ', $('#selectedUserName').val());
		// console.log('includeSubDept: ', $('#includeSubDept').is(':checked') ? true : false);

		const searchConditions = {
			userType: userSelectOption.val(),
			syncType: hrSelectOption.val(),
			deptId: null,
			searchKeyword: $('#selectedUserName').val(),
			includeSubDept: $('#includeSubDept').is(':checked') ? true : false
		};

		const columns = [
			{ data: 'deptName' },  // 부서 이름
			{ data: 'userNm' },    // 사용자 이름
			{ data: 'emplyrId' }   // 사용자 ID
		];

		loadDataTable(deptUserTable, '#deptUserTable', searchConditions, columns, null);

		// 검색 팝업 숨기기
		$('.searchArea_p_Popup.userSearchPopup').addClass('hide');
	}
}

function removeApprovalLineFilter(element, popupName) {
	// 선택된 필터의 filterTitle을 찾아서 초기화함수 호출
	var filterTitle = $(element).closest('.filterItem').find('.filterTitle').text();
	resetApprovalLineSearchValue(popupName, filterTitle);
	$(element).closest('.filterItem').remove(); // 해당 필터 아이템 제거

	if(popupName == "searchArea_p_Popup departSearchPopup") {
		loadAndRenderDeptTree('departTreeMap', 'deptTreeContainer', null, '', loadUserList);
	}
	else if(popupName == "searchArea_p_Popup userSearchPopup") {
		var userSelectOption = $('#commonUserSelectOption');
		var hrSelectOption = $('#commonHrSelectOption');
	
		const searchConditions = {
			userType: userSelectOption.val(),
			syncType: hrSelectOption.val(),
			searchKeyword: searchKeyword,
			includeSubDept: $('#includeSubDept').is(':checked') ? true : false
		};

		const columns = [
			{ data: 'deptName' },  // 부서 이름
			{ data: 'userNm' },    // 사용자 이름
			{ data: 'emplyrId' }   // 사용자 ID
		];

		loadDataTable(deptUserTable, '#deptUserTable', searchConditions, columns, null);
	}

}

function resetApprovalLineSearchValue(popupName, filterTitle) {
	
	if(popupName == "searchArea_p_Popup departSearchPopup") {
		var searchKeyword = $('#commonDeptSearchKeyword');
		if(filterTitle == "부서") {
			searchKeyword.val('');
		}
	}
	else if(popupName == "searchArea_p_Popup userSearchPopup") {
		var userSelectOption = $('#commonUserSelectOption');
		var hrSelectOption = $('#commonHrSelectOption');

		if(filterTitle == "사용자") {
			userSelectOption.val('');
		}
		else if(filterTitle == "구분") {
			hrSelectOption.val('');
		}
	}
}

/**
 * 사용자 목록 로드
 */
function loadUserList(deptName, deptId, companyId) {

	// 하위 부서 포함 체크 박스 결과를 가져옴
	var includeSubDept = $('#includeSubDept').is(':checked') ? true : false;
	// console.log('includeSubDept: [' + includeSubDept + ']');

	const searchConditions = {
		//searchKeyword: deptName, // 검색어 추가
		deptId: deptId,       // 부서 ID 추가
		includeSubDept: includeSubDept // 하위 부서 포함 여부
	};

	const columns = [
		{ data: 'deptName' },  // 부서 이름
		{ data: 'userNm' },    // 사용자 이름
		{ data: 'emplyrId' }   // 사용자 ID
	];

	loadDataTable(deptUserTable, '#deptUserTable', searchConditions, columns, null);
}

/** 서식 로드 시, 권한에 따라 사용할 수 있는 서식이 없는 경우 파라미터로 받은 class에 disabled 추가
 * @param {string} targetSelector - 대상 선택자
 */
async function setMenuBtn(targetSelector) {
	console.log('targetSelector: ', targetSelector);

	// 사용자 권한 로드
	const myInfo = await loadMyInfo();
	const userRole = myInfo.userRole;
	var notice = await loadNotice();
	var adminCount = 0;
	var userCount = 0;

	for(var i = 0; i < notice.data.length; i++) {
		console.log('notice.data[i]: ', notice.data[i]);
		if(notice.data[i].requestUserRole == 'ADMIN' && userRole.includes('ADMIN') && notice.data[i].useYn == 'Y') {
			adminCount++;
		}
		else if(notice.data[i].requestUserRole == 'USER' && userRole.includes('USER') && notice.data[i].useYn == 'Y') {
			userCount++;
		}
	}
	console.log('adminCount: ', adminCount);
	console.log('userCount: ', userCount);
	console.log('userRole: ', userRole);

	if (userRole.includes('ADMIN') && adminCount > 0) {
	    $(targetSelector).removeClass('disabled').prop('disabled', false).closest('li').removeClass('disable'); // <li>의 disable 클래스 제거
	    $(".profilePopup .accountExtensionBtn").removeClass("hide");
	} 
	else if (userRole.includes('USER') && userCount > 0) {
	    $(targetSelector).removeClass('disabled').prop('disabled', false).closest('li').removeClass('disable'); // <li>의 disable 클래스 제거
	    $(".profilePopup .accountExtensionBtn").removeClass("hide");
	}


}

/**
 * 공지사항 로드
 */
async function loadNotice() {
	const response = await $.ajax({
		url: '/primx/fun/cmm/createRequestSetting/list.do',
		type: 'GET'
	});
	
	return response;
}

/**
 * 서식 사용 여부 설정
 */
async function setFormUseYn(userRole, targetSelector) {
	// select #requestApprovalCode의 옵션값 중 사용 여부가 N인 경우 서식 숨김
	// 모든 옵션에 class hide 추가
	$(targetSelector + ' .typeOfApplication option').addClass('hide');
	// 모든 서식 숨기기
	$(targetSelector + ' .approvalFormTypeM').addClass('hide');
	$(targetSelector + ' .approvalFormTypeC').addClass('hide');
	$(targetSelector + ' .approvalFormTypeA').addClass('hide');

	var notice = await loadNotice();
	for(var i = 0; i < notice.data.length; i++) {
		// N이 아닌 경우 서식 보이기
		if(notice.data[i].useYn == 'Y') {
			if(userRole.includes('ADMIN') && notice.data[i].requestUserRole == 'ADMIN') {
				$(targetSelector + ' .typeOfApplication option[value="' + notice.data[i].requestApprovalCode + '"]').removeClass('hide');
				$(targetSelector + ' .typeOfApplication option[value="' + notice.data[i].requestApprovalCode + '"]').prop('selected', true);
				if(notice.data[i].requestApprovalCode == 'M') {
					$(targetSelector + ' .approvalFormTypeM').removeClass('hide');
					$(targetSelector + ' .approvalFormTypeC').addClass('hide');
					$(targetSelector + ' .approvalFormTypeA').addClass('hide');
				}
				else if(notice.data[i].requestApprovalCode == 'C') {
					$(targetSelector + ' .approvalFormTypeM').addClass('hide');
					$(targetSelector + ' .approvalFormTypeC').removeClass('hide');
					$(targetSelector + ' .approvalFormTypeA').addClass('hide');
				}
				else if(notice.data[i].requestApprovalCode == 'A') {
					$(targetSelector + ' .approvalFormTypeM').addClass('hide');
					$(targetSelector + ' .approvalFormTypeC').addClass('hide');
					$(targetSelector + ' .approvalFormTypeA').removeClass('hide');
				}
			}
			else if(userRole.includes('USER') && notice.data[i].requestUserRole == 'USER') {
				$(targetSelector + ' .typeOfApplication option[value="' + notice.data[i].requestApprovalCode + '"]').removeClass('hide');
				$(targetSelector + ' .typeOfApplication option[value="' + notice.data[i].requestApprovalCode + '"]').prop('selected', true);
				if(notice.data[i].requestApprovalCode == 'M') {
					$(targetSelector + ' .approvalFormTypeM').removeClass('hide');
					$(targetSelector + ' .approvalFormTypeC').addClass('hide');
					$(targetSelector + ' .approvalFormTypeA').addClass('hide');
				}
				else if(notice.data[i].requestApprovalCode == 'C') {
					$(targetSelector + ' .approvalFormTypeM').addClass('hide');
					$(targetSelector + ' .approvalFormTypeC').removeClass('hide');
					$(targetSelector + ' .approvalFormTypeA').addClass('hide');
				}
				else if(notice.data[i].requestApprovalCode == 'A') {
					$(targetSelector + ' .approvalFormTypeM').addClass('hide');
					$(targetSelector + ' .approvalFormTypeC').addClass('hide');
					$(targetSelector + ' .approvalFormTypeA').removeClass('hide');
				}
			}
			
		}
	}
}

/**
 * 결재대상 설정
 */
async function setApprovalTarget(userRole, requestApprovalCode) {
	var notice = await loadNotice();
	for(var i = 0; i < notice.data.length; i++) {
		if(notice.data[i].useYn == 'Y') {
			if(notice.data[i].requestUserRole == 'ADMIN' && userRole.includes('ADMIN')) {
				if(notice.data[i].requestApprovalCode == requestApprovalCode) {
					return notice.data[i].approvTarget;
				}
			}
			else if(notice.data[i].requestUserRole == 'USER' && userRole.includes('USER')) {
				if(notice.data[i].requestApprovalCode == requestApprovalCode) {
					return notice.data[i].approvTarget;
				}
			}
		}
	}
}

/**
 * 공지사항 설정
 */
async function setNotice(userRole, targetSelector) {
	var notice = await loadNotice();
	for(var i = 0; i < notice.data.length; i++) {
		if(notice.data[i].requestUserRole == 'ADMIN' && userRole.includes('ADMIN')) {
			if(notice.data[i].requestApprovalCode == 'M') {
				$(targetSelector+'M').val(notice.data[i].noticeContent);
			}
			else if(notice.data[i].requestApprovalCode == 'C') {
				$(targetSelector+'C').val(notice.data[i].noticeContent);
			}
			else if(notice.data[i].requestApprovalCode == 'A') {
				$(targetSelector+'A').val(notice.data[i].noticeContent);
			}
		}
		else if(notice.data[i].requestUserRole == 'USER' && userRole.includes('USER')) {
			if(notice.data[i].requestApprovalCode == 'M') {
				$(targetSelector+'M').val(notice.data[i].noticeContent);
			}
			else if(notice.data[i].requestApprovalCode == 'C') {
				$(targetSelector+'C').val(notice.data[i].noticeContent);
			}
			else if(notice.data[i].requestApprovalCode == 'A') {
				$(targetSelector+'A').val(notice.data[i].noticeContent);
			}
		}
	}
}

/**
 * 사유 설정
 */
async function setReason(userRole, targetSelector) {
	var notice = await loadNotice();
	for(var i = 0; i < notice.data.length; i++) {
		if(notice.data[i].reasonUsedYn == 'Y') {
			if(notice.data[i].requestUserRole == 'ADMIN' && userRole.includes('ADMIN')) {
				if(notice.data[i].requestApprovalCode == 'M') {
					$(targetSelector+'M').val(notice.data[i].noticeReason);
				}
				else if(notice.data[i].requestApprovalCode == 'C') {
					$(targetSelector+'C').val(notice.data[i].noticeReason);
				}
				else if(notice.data[i].requestApprovalCode == 'A') {
					$(targetSelector+'A').val(notice.data[i].noticeReason);
				}
			}
			else if(notice.data[i].requestUserRole == 'USER' && userRole.includes('USER')) {
				if(notice.data[i].requestApprovalCode == 'M') {
					$(targetSelector+'M').val(notice.data[i].noticeReason);
				}
				else if(notice.data[i].requestApprovalCode == 'C') {
					$(targetSelector+'C').val(notice.data[i].noticeReason);
				}
				else if(notice.data[i].requestApprovalCode == 'A') {
					$(targetSelector+'A').val(notice.data[i].noticeReason);
				}
			}
		}
	}
}


// 알림 기능


$(document).ready(function () {
    // 알림 목록 관련 요소 선택
    const $alarmPopup = $(".topAlarmPopup");
    const $alarmWrap = $alarmPopup.find(".alarmWrap");
    const $alarmItems = $alarmWrap.find("li");
    const $moreButton = $alarmPopup.find(".moreBtnWrap button:first-child"); // "더보기" 버튼
    const $allViewStatusBtn = $(".allViewStatusBtn"); // 전체 읽음 버튼

    const maxVisibleItems = 6;  // 초기 표시 개수
    const loadMoreCount = 10;   // 한 번에 추가로 보여줄 개수
    let currentVisibleCount = maxVisibleItems; // 현재 표시된 개수

    // 처음에는 6개만 표시, 나머지는 숨김
    $alarmItems.each(function (index) {
        if (index >= maxVisibleItems) {
            $(this).hide();
        }
    });

    // "더보기" 버튼 클릭 시 추가 10개 로드
    $moreButton.on("click", function () {
        loadMoreItems();
        $(".topAlarmPopup .moreBtnWrap").hide();
        $(".alarmWrap").addClass("readAll");
    });

    // 스크롤이 끝에 닿으면 자동으로 추가 10개 로드
    $alarmWrap.on("scroll", function () {
        let scrollHeight = $alarmWrap[0].scrollHeight;
        let scrollTop = $alarmWrap.scrollTop();
        let clientHeight = $alarmWrap.outerHeight();

        if (scrollTop + clientHeight >= scrollHeight - 10) {
            loadMoreItems();
        }
    });

    // 알림 추가 로드 함수
    function loadMoreItems() {
        let newVisibleCount = currentVisibleCount + loadMoreCount;

        // 새로운 개수만큼 보이게 설정
        $alarmItems.each(function (index) {
            if (index < newVisibleCount) {
                $(this).fadeIn(200); // 부드럽게 나타나도록 설정
            }
        });

        // 업데이트된 개수 저장
        currentVisibleCount = newVisibleCount;

        // 모든 항목이 보이면 버튼 숨김 & 스크롤 이벤트 제거
        if (currentVisibleCount >= $alarmItems.length) {
            $alarmWrap.off("scroll"); // 더 이상 이벤트 발생 안 하도록 해제
        }
    }

    // 개별 알림의 status 클릭 시 해당 li에 'view' 클래스 추가
    $alarmWrap.on("click", ".status", function () {
		// li 에서 seqNo 가져오기
		var seqNo = $(this).closest("li").find(".seqNo").val();
		// 알림 읽음 처리
		readAlarm(seqNo);
		// 알림 카운트 감소
		var $alarmCount = $("#alarmCount");
		$alarmCount.text(parseInt($alarmCount.text()) - 1);
        $(this).closest("li").addClass("view");
    });
    // 개별 알림의 cont(해당페이지로) 클릭 시 해당 li에 'view' 클래스 추가
    $alarmWrap.on("click", "a", function () {
		// li 에서 seqNo 가져오기
		var seqNo = $(this).closest("li").find(".seqNo").val();
		// 알림 읽음 처리
		readAlarm(seqNo);
		// 알림 카운트 감소
		var $alarmCount = $("#alarmCount");
		$alarmCount.text(parseInt($alarmCount.text()) - 1);
        $(this).closest("li").addClass("view");
    });

    // "전체 읽음" 버튼 클릭 시 모든 li에 'view' 클래스 추가
    $allViewStatusBtn.on("click", function () {
		readAllAlarm();
		// 알림 카운트 0으로 설정
		var $alarmCount = $("#alarmCount");
		$alarmCount.text(0);
        $(".alarmWrap li").addClass("view");
        $(".newNoti").addClass("hide");
    });
	
});

/**
 * 알림 생성
 * @param {Object} data 알림 정보
 * @returns {jQuery} 생성된 알림 요소
 */
function createAlarm(data) {
	console.log("data: ", data);

	// 시퀀스 코드를 히든으로 추가
	var $seqNo = $('<input type="hidden" class="seqNo" value="' + data.seqNo + '">');
	var type = data.type;
	var typeDesc = data.typeDesc;
	var txt = '';
	var insertDate = data.insertDate;

	// 읽음 여부 확인
	var isRead = data.isRead;
	
	var $alarm = $('<li>');
	if(isRead == 1) {
		$alarm.addClass('view');
	}

	var $status = $('<div class="status"></div>');
	var $typeCir = null;
	switch(type) {
		case 'MD':
		case 'M':
		case 'C':
		case 'A':
			$typeCir = $('<div class="typeCir approval"></div>');
			txt = data.userNm + ' ' + typeDesc;
			break;
		case 'E0001':
			$typeCir = $('<div class="typeCir profile"></div>');
			txt = data.userNm + ' ' + typeDesc;
			break;
		case 'H0001':
		case 'D0001':
			$typeCir = $('<div class="typeCir sys"></div>');
			txt = typeDesc;
			break;
	}
	// path가 전체 경로인 경우 그대로 사용
	var $cont = data.path ? 
		$('<a class="cont menu-link" href="' + contextPath + data.path + '"></a>') : 
		$('<a class="cont"></a>');
	var $txt = $('<p class="txt">' + txt + '</p>');
	var $date = $('<p class="date">' + new Date(insertDate).toLocaleString() + '</p>');

	$alarm.append($status);
	$alarm.append($typeCir);
	$alarm.append($cont);
	$cont.append($txt);
	$cont.append($date);
	$alarm.append($seqNo);

	return $alarm;
}

async function loadAlarm() {

	var count = 0;
	// 알림 카운트
	var $alarmCount = $("#alarmCount");
	// 초기에 hide 클래스 적용
	$alarmCount.addClass('hide');

	var response = await $.ajax({
		url: '/primx/fun/cmm/alarm/list.do',
		type: 'GET',
		success: function(response) {
			var alarmList = response.alarmList;
			var totalCount = response.totalCount;
			console.log("alarmList: ", alarmList);
			for(var i = 0; i < alarmList.length; i++) {
				var alarm = alarmList[i];
				var $alarm = createAlarm(alarm);
				$("#alarmWrap").append($alarm);
				count += alarm.isRead == 0 ? 1 : 0;
			}
			$alarmCount.text(count);
			if(count > 0) {
				$alarmCount.removeClass('hide');
			}
		},
		error: function(error) {
			console.log(error);
		}
	});
}


function initAlarm() {
	// 알림 목록 초기화
	$("#alarmWrap").empty();
	// 알림 목록 로드
	loadAlarm();
	$('.topAlarmPopup').removeClass('hide');
}

function readAlarm(seqNo) {
	$.ajax({
		url: '/primx/fun/cmm/alarm/read.do',
		type: 'POST',
		data: { seqNo: seqNo },
		success: function(response) {
			console.log("response: ", response);
		},
		error: function(error) {
			console.log(error);
		}
	});
}

function readAllAlarm() {
	$.ajax({	
		url: '/primx/fun/cmm/alarm/readAll.do',
		type: 'POST',
		success: function(response) {
			console.log("response: ", response);
		},
		error: function(error) {
			console.log(error);
		}
	});
}

// XZED 스마트 컨트랙트 설정 팝업 열기
function openXZEDSmartContractSettingPopup() {
	// 부서 트리 로드
	loadSmartContractDeptTree();
	loadSmartContractFileTable();

    // smartContractUserTable 초기화
	if(smartContractUserTable != null) {
		smartContractUserTable.clear().destroy();
	}
	$('#xzedSmartContractSettingTable tbody').empty();

	$('.popup.xzedSmartContractSettingPopup').removeClass('hide');
}

var smartContractFileListTable = null;
// 스마트 컨트랙트 파일 목록 테이블 로드
function loadSmartContractFileTable() {

	const searchOption = {
        requestApprovalCode: 'M',
        statusCd: 'PE'
    };

	smartContractFileListTable = initDataTables('#smartContractFileListTable' ,{
		ajax: {
			url: '/primx/fun/rqt/createRequest/list.do',
			type: 'POST',
			data: function(data) {
				data.requestApprovalCode = searchOption.requestApprovalCode;
				data.statusCd = searchOption.statusCd;
			}
		},
		info : false,
		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: null,
			bottomEnd: {
				paging: {
					buttons: 5
				},
			},
		},
		processing: true,
    	serverSide: true,
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
	        { data: 'title' },
	        { data: 'userNm' }
	    ],
		columnDefs: [
			{
				render: DataTable.render.select(),
				targets: 0
			}
		],
		select: {
			style: 'single',
			selector: 'input.row-checkbox'
		}
	});

	// 단일 선택 체크박스 이벤트
	$('#smartContractFileListTable tbody').on('change', '.row-checkbox', function() {
	    $('.row-checkbox').not(this).prop('checked', false); // 다른 체크박스 해제
	    $('#smartContractFileListTable tbody tr').removeClass('selected'); // 모든 행에서 선택 효과 제거
	
	    if ($(this).prop('checked')) {
	        $(this).closest('tr').addClass('selected'); // 선택된 행에 배경색 적용
	    }
	});
	
}
// XZED 스마트 컨트랙트 설정 팝업 닫기
function closeXZEDSmartContractSettingPopup() {
	$('.popup.xzedSmartContractSettingPopup').addClass('hide');
}


function loadSmartContractDeptTree() {

	$.ajax({
		url: '/primx/fun/dpt/tree.do',
		type: 'POST',
		data: {
			searchKeyword: '',
			parentId: ''
		},
		success: function(response) {
			if (response && response.data) {
				
				// 부서 트리 컨테이너 선택
				const container = document.querySelector('.xzedSmartContractSettingPopup .departTreeMap');
				if (!container) return;

				const tree = new CustomTree(container, {
					expandAll: false,
					onSelect: function(deptName, deptId, companyId) {
						loadSmartContractUserTable(deptId);
					}
				});
				tree.render(response.data);
			} else {
				console.log("response.data: ", response.data);
			}
		},
		error: function(xhr, status, error) {
			console.error("부서 트리 로드 실패:", error);
		}
	});

}

let smartContractUserTable = null;

function loadSmartContractUserTable(deptId) {
	
	var tableSelector = '#smartContractUserTable';
	// smartContractUserTable body 초기화
	$('#smartContractUserTable tbody').empty();

	const searchConditions = {
		deptId: deptId,
		includeSubDept: true
	};
	const columns = [
		// 회사명, 성명, 메일계정
		{
			data: 'deptName'
		},
		{
			data: 'userNm'
		},
		{
			data: 'emailAdres'
		},
		{
			data: 'agentId'
		}
	];

	const additionalOptions = null;

	// 기본 설정
	let tableOptions = {
		ajax: {
			url: '/primx/fun/cmm/smartContractUserList.do',
			type: 'POST',
			data: function(d) {
				// 검색 조건 추가
				for (const key in searchConditions) {
					d[key] = searchConditions[key];
				}
				// 페이지 번호와 페이지 크기 추가
				d.page = d.start / d.length + 1;
				d.pageSize = d.length;
			}
		},
		processing: true,
		serverSide: true,
		columns: columns
	};

	// 추가 옵션 병합
	if (additionalOptions) {
		for (const key in additionalOptions) {
			if (additionalOptions[key] !== null && additionalOptions[key] !== undefined) {
				tableOptions[key] = additionalOptions[key];
			}
		}
	}

	// 데이터테이블 초기화 및 반환
	const initializedTable = initDataTables(tableSelector, tableOptions);

	// 더블클릭 이벤트 핸들러 설정
	$(tableSelector + ' tbody').off('dblclick');
	$(tableSelector + ' tbody').on('dblclick', 'td', function() {
		const data = initializedTable.row(this).data();
		if (data) {
			console.log("data: ", data);
			addSmartContractUser(data);
		}
	});
	
}

function addSmartContractUser(data) {
    // agentId가 이미 존재하는지 확인
    var existingAgentId = false;
    $('#xzedSmartContractSettingTable tbody tr').each(function() {
        if ($(this).find('input[name="agentId"]').val() === data.agentId) {
            existingAgentId = true;
            return false; // each 루프 종료
        }
    });

    if (existingAgentId) {
        alert('이미 등록된 사용자입니다.');
        return;
    }

    // xzedSmartContractSettingTable
    var $table = $('#xzedSmartContractSettingTable tbody');
    
    // 새로운 row 생성
    var $newRow = $('<tr>');
    
    // 회사명 td에 hidden input으로 agentId와 emplyrId 추가
    var $deptNameTd = $('<td>').text(data.deptName);
    $deptNameTd.append($('<input>').attr({
        type: 'hidden',
        name: 'agentId',
        value: data.agentId || ''
    }));
    $deptNameTd.append($('<input>').attr({
        type: 'hidden',
        name: 'emplyrId',
        value: data.emplyrId || ''
    }));
    $newRow.append($deptNameTd);
    
    // 나머지 컬럼 추가
    $newRow.append($('<td>').text(data.userNm)); // 성명
    $newRow.append($('<td>').text(data.emailAdres)); // 이메일
    
    // 전화번호 컬럼 (data에 phoneNo가 없는 경우 빈 문자열로 표시)
    $newRow.append($('<td>').text(data.phoneNo || ''));
    
    // 기간설정 컬럼
    var $periodCell = $('<td>');
    $periodCell.append(`
        <div class="selectBox wAuto">
            <select class="drop xzedPeriodSelect">
                <option value="" data-i18n="xzedSmartContractSettingPopup.setPeriod">기간 지정</option>
                <option value="3" data-i18n="xzedSmartContractSettingPopup.3month">3개월</option>
                <option value="6" data-i18n="xzedSmartContractSettingPopup.6month">6개월</option>
                <option value="12" data-i18n="xzedSmartContractSettingPopup.12month">12개월</option>
                <option value="0" data-i18n="xzedSmartContractSettingPopup.unlimited">무기한</option>
            </select>
        </div>
        <input type="text" class="datepicker txtBox xzedStartDate" readonly data-i18n-placeholder="datepicker.startDate">
        <input type="text" class="datepicker txtBox xzedEndDate" readonly data-i18n-placeholder="datepicker.endDate">
    `);
    $newRow.append($periodCell);
    
    // IP, MAC 컬럼 (data에 없는 경우 빈 문자열로 표시)
    $newRow.append($('<td>').text(data.ip || ''));
    $newRow.append($('<td>').text(data.mac || ''));
    
    // 삭제버튼 컬럼
    var $deleteCell = $('<td>');
    $deleteCell.append(`
        <div class="bu_wrap mr10">
            <button class="" type="button" data-i18n="button.delete">삭제</button>
        </div>
    `);
    $newRow.append($deleteCell);
    
    // 테이블에 새로운 row 추가
    $table.append($newRow);
 
    applyTranslations();
    
    // datepicker 초기화
    $newRow.find('.datepicker').datepicker({
        dateFormat: 'yy-mm-dd',
        onSelect: function(dateText, inst) {
            $(this).datepicker('hide');
            $(this).closest('td').find('.xzedPeriodSelect').val('');
        }
    });
    
    // 기간 선택 이벤트 핸들러
    $newRow.find('.xzedPeriodSelect').on('change', function() {
        var selectedValue = $(this).val();
        var today = new Date();
        var $startDate = $(this).closest('td').find('.xzedStartDate');
        var $endDate = $(this).closest('td').find('.xzedEndDate');
        
        if (selectedValue === '0') {
            $startDate.hide();
            $endDate.val('9999-12-31').prop('disabled', true);
        } else if (selectedValue) {
            $startDate.show().val($.datepicker.formatDate('yy-mm-dd', today));
            $endDate.prop('disabled', false);
            
            var endDate = new Date(today);
            endDate.setMonth(today.getMonth() + parseInt(selectedValue));
            $endDate.val($.datepicker.formatDate('yy-mm-dd', endDate));
        } else {
            $startDate.show().val('');
            $endDate.val('').prop('disabled', false);
        }
    });
    
    // 삭제 버튼 이벤트 핸들러
    $newRow.find('button[data-i18n="button.delete"]').on('click', function() {
        $(this).closest('tr').remove();
    });
}

// XZED 스마트 컨트랙트 설정 팝업의 기간 선택에 따른 날짜 처리
$(document).ready(function() {
    // datepicker 초기화
    $('.xzedStartDate, .xzedEndDate').datepicker({
        dateFormat: 'yy-mm-dd',
        onSelect: function(dateText, inst) {
            $(this).datepicker('hide');
            // datepicker에서 날짜를 선택하면 셀렉트 박스를 기간 지정으로 변경
            $('.xzedPeriodSelect').val('');
        }
    });

    // 셀렉트 박스 변경 이벤트
    $('.xzedPeriodSelect').on('change', function() {
        var selectedValue = $(this).val();
        var today = new Date();
        
        if (selectedValue === '0') {
            // 무기한인 경우
            $('.xzedStartDate').hide();
            $('.xzedEndDate').val('9999-12-31').prop('disabled', true);
        } else if (selectedValue) {
            // 3, 6, 12개월 선택시
            $('.xzedStartDate').show().val($.datepicker.formatDate('yy-mm-dd', today));
            $('.xzedEndDate').prop('disabled', false);
            
            var endDate = new Date(today);
            endDate.setMonth(today.getMonth() + parseInt(selectedValue));
            $('.xzedEndDate').val($.datepicker.formatDate('yy-mm-dd', endDate));
        } else {
            // 기간 지정 선택시
            $('.xzedStartDate').show().val('');
            $('.xzedEndDate').val('').prop('disabled', false);
        }
    });
});

function applyXZEDSmartContractSetting() {
    // smartContractFileListTable에서 선택된 row 가져오기
    const selectedRows = smartContractFileListTable.rows({ selected: true }).data();
    
    // 선택된 파일이 없는 경우
    if (selectedRows.length === 0) {
        alert('스마트 컨트랙트 파일을 선택해주세요.');
        return;
    }
    
    // 선택된 파일 정보 로그 출력
    console.log('선택된 파일 정보:', selectedRows[0]);

    // xzedSmartContractSettingTable에 등록된 모든 row 가져오기
    const allRows = $('#xzedSmartContractSettingTable tbody tr');
    
    // 등록된 사용자가 없는 경우
    if (allRows.length === 0) {
        alert('스마트 컨트랙트 사용자를 추가해주세요.');
        return;
    }

    // 각 row의 데이터를 리스트로 만들기
    const userList = [];
    allRows.each(function() {
        const $row = $(this);
        const userData = {
			fileName: selectedRows[0].title,
            agentId: $row.find('input[name="agentId"]').val(), // hidden input에서 agentId 가져오기
            emplyrId: $row.find('input[name="emplyrId"]').val() // hidden input에서 emplyrId 가져오기
        };
        userList.push(userData);
    });

    // 사용자 리스트 로그 출력
    console.log('등록된 사용자 리스트:', userList);
    
    // TODO: 서버로 데이터 전송하는 로직 추가
	$.ajax({
		url: '/primx/fun/sup/applyXZEDSmartContractSetting.do',
		type: 'POST',
		contentType: 'application/json',  // JSON 형식으로 전송
		data: JSON.stringify(userList),   // JSON 문자열로 변환
		success: function(response) {
			if (response.result) {
				alert('스마트 컨트랙트 설정이 적용되었습니다.');
				closeXZEDSmartContractSettingPopup();
			} else {
				alert('스마트 컨트랙트 설정에 실패했습니다.');
			}
		},
		error: function(xhr, status, error) {
			console.error('오류:', error);
		}
	});
}

/**
 * 트리 데이터를 CSV로 변환하여 다운로드
 * @param {Array} treeData - 트리 데이터
 * @param {string} fileName - 다운로드할 파일 이름
 */
function exportTreeToCsv(treeData, fileName) {
    // 트리 구조를 먼저 구성
    function buildTreeStructure(data) {
        var map = {};
        var roots = [];

        // 모든 노드를 맵으로 변환
        for (var i = 0; i < data.length; i++) {
            var item = data[i];
            map[item.deptId] = {
                deptId: item.deptId,
                deptName: item.deptName,
                parentId: item.parentId,
                level: item.level,
                orderIndex: item.orderIndex,
                companyId: item.companyId,
                children: []
            };
        }

        // 부모-자식 관계 구성
        for (var i = 0; i < data.length; i++) {
            var item = data[i];
            if (item.parentId && map[item.parentId]) {
                map[item.parentId].children.push(map[item.deptId]);
            } else {
                roots.push(map[item.deptId]);
            }
        }

        // level과 orderIndex 기준으로 정렬
        roots.sort(function(a, b) {
            if (a.level !== b.level) {
                return a.level - b.level;
            }
            return (a.orderIndex || 0) - (b.orderIndex || 0);
        });

        return roots;
    }

    // CSV 내용 생성
    let csvContent = '\ufeff'; // UTF-8 BOM 추가
    
    // 헤더 행 추가
    csvContent += '부서명,회사코드,부서코드\n';
    
    // 트리 데이터를 CSV 형식으로 변환
    function processNode(node, level = 0) {
        // 들여쓰기를 위한 공백 생성 (웹 UI와 동일하게)
        const indent = '    '.repeat(level); // 4칸 공백으로 들여쓰기
        const deptName = indent + node.deptName;
        const companyId = node.companyId || '';
        const deptId = node.deptId || '';
        
        // 각 필드를 쉼표로 구분하고 필드 내 쉼표를 처리하기 위해 쌍따옴표로 감싸기
        csvContent += `"${deptName}","${companyId}","${deptId}"\n`;
        
        // 자식 노드가 있으면 정렬 후 재귀적으로 처리
        if (node.children && node.children.length > 0) {
            // orderIndex 기준으로 자식 노드 정렬
            node.children.sort((a, b) => (a.orderIndex || 0) - (b.orderIndex || 0));
            
            node.children.forEach(child => {
                processNode(child, level + 1);
            });
        }
    }
    
    // 트리 구조 구성
    const treeStructure = buildTreeStructure(treeData);
    
    // 각 루트 노드부터 처리 시작
    treeStructure.forEach(node => {
        processNode(node);
    });
    
    // Blob 생성 및 다운로드
    const blob = new Blob([csvContent], { 
        type: 'application/vnd.ms-excel;charset=utf-8'
    });
    const url = window.URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.style.display = 'none';
    a.href = url;
    a.download = fileName;
    document.body.appendChild(a);
    a.click();
    window.URL.revokeObjectURL(url);
    document.body.removeChild(a);
}