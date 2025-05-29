/* 설정 > 부서 및 사용자 설정 */
let userTable = null;
let selectedDeptId = null;
let selectedDeptName = null;
let selectedcompanyId = null;
let deptTree = null;
let isDeptIdValid = false; // 사용자 ID 유효성 플래그

/* csv 다운로드 제목포멧 */
var firstDepthTit = $(".left nav .firstDepthMenu > li.select > p").text();
var secondDepthTit = $(".left nav .firstDepthMenu > li.select .secondDepthMenu > li.select a").text();
var today = new Date();
var formattedDate = today.getFullYear() + '-' +
    ('0' + (today.getMonth() + 1)).slice(-2) + '-' +
    ('0' + today.getDate()).slice(-2);
    
// 검색 조건 팝업(부서)
$(".departUserSection .departmentArea .searchInput2").on("click",function(){
	$(".departSearchPopup2").removeClass("hide");
	$(".departSearchPopup2 #deptNameSearch").focus();
})
// 검색 조건 팝업(사용자)
$(".departUserSection .userArea .searchInput2").on("click",function(){
	$(".userSearchPopup2").removeClass("hide");
	$(".userSearchPopup2 #selectedUserName").focus();
})

function pageInit(){
	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();

	/* input file 커스텀 */
	inputFileCustom();

	/* 부서 트리 로드 */
	loadDeptTree();
	/* 사용자 테이블 로드 */
	/* dataTable */
	userTable = initDataTables('#userTable');
	loadUserTable();

	/* 사용자 이벤트 리스너 */
	initUsePeriodListeners();
	initUserEventListeners();

	// #includeSubDept 체크박스 이벤트 리스너
	$("#includeSubDept").on("click", function() {
		console.log('includeSubDept: ', $(this).is(":checked") ? true : false);
		loadUserTable();
	});
}

function loadUserTable(){

	// console.log('includeSubDept: ', $("#includeSubDept").is(":checked") ? true : false);
	
	const searchConditions = {
		userType: $(".userSearchPopup2 select[name='userType']").val(),
		syncType: $(".userSearchPopup2 select[name='syncType']").val(),
		deptId: selectedDeptId,
		searchKeyword: $("#selectedUserName").val(),
		includeSubDept: $("#includeSubDept").is(":checked") ? true : false
	};

	const columns = [
		{
			data: null
		},
		{ data: null, render: function(data, type, row, meta) {
			return meta.row + meta.settings._iDisplayStart + 1;
		}},
		{ data: 'emplyrId' },
		{
			data: 'userRoleNm'
		},
		{ data: 'userNm' },
		{ data: 'emailAdres' },
		{ data: 'deptName' },
		{ 
			data: 'insertByYn',
			render: function(data) {
				return data === 'Y' ? '연동' : '수동';
			}
		},
		{ 
			data: 'userYn',
			render: function(data) {
				return data === 'Y' ? '활성' : '비활성';
			}
		},
		{ 
			data: 'pwdChanged',
			render: function(data) {
				return data === 'Y' ? '변경완료' : '초기비밀번호';
			}
		},
		{ 
			data: 'expiredDate', 
			render: function(data) {
				if (!data) return '';
				if (data.includes('9999')) {
					return '무기한';
				}
				return new Date(data).toLocaleDateString('ko-KR', {
					year: 'numeric',
					month: '2-digit',
					day: '2-digit'
				}).replace(/\. /g, '-').replace('.', '');
			}
		},
		{ data: 'insertDate', render: function(data) {
			return new Date(data).toLocaleString();
		}}
	];

	const additionalOptions = {
		columnDefs: [{
			render: DataTable.render.select(),
			targets: 0
		}],
		buttons: [{
			extend: 'csv',
			title: `${firstDepthTit}_${secondDepthTit}_${formattedDate}`,
			exportOptions: {
				columns: ':not(:first-child)'  // 첫 번째 컬럼(체크박스)을 제외한 모든 컬럼 내보내기
			},
			bom: true,
			customize: function(csv) {
				return '\ufeff' + csv;
			}
		}],
		select: {
			style: 'multi',
			selector: 'td:first-child'
		}
	};

	// loadDataTable의 반환값을 userTable에 할당
	userTable = loadDataTable(userTable, '#userTable', searchConditions, columns, additionalOptions);
	
	// 데이터 테이블의 행 클릭 이벤트 추가
	$('#userTable tbody').off('click', 'td:not(:first-child)');  // 이벤트 바인딩 방식 변경
	$('#userTable tbody').on('click', 'td:not(:first-child)', function(e) {
		const tr = $(this).closest('tr');
		const data = userTable.row(tr).data();  // tr 요소를 사용하여 데이터 가져오기
		if (data) {
			loadUserDetails(data.emplyrId);
		}
	});

	// CSV 내보내기 버튼과 연결
    $('#departmentUserSettingsCsvExport').on('click', function() {
        userTable.button('.buttons-csv').trigger();
    });
}

// 선택된 사용자 ID 배열 반환
function getSelectedUserIds() {
	console.log('getSelectedUserIds 호출');
	const selectedUsers = [];
	$('#userTable tbody .selected').each(function() {
		const rowData = userTable.row($(this).closest('tr')).data();
		if (rowData && rowData.emplyrId) {
			selectedUsers.push(rowData.emplyrId);
		}
	});
	return selectedUsers;
}

// 선택된 사용자 ID에 해당하는 이름을 반환하는 함수
function getSelectedUserNames() {
	const selectedUsers = [];
	$('#userTable tbody .selected').each(function() {
		const rowData = userTable.row($(this).closest('tr')).data();
		if (rowData && rowData.userNm) {
			selectedUsers.push(rowData.userNm);
		}
	});
	return selectedUsers;
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
			console.log('deptName: ', deptName);
			console.log('부서 선택됨:', {
				selectedDeptId: selectedDeptId,
				selectedDeptName: selectedDeptName,
				companyId: companyId
			});

			// 부서 선택 팝업이 열려있는 경우에만 즉시 적용
			if (!$('.departSelectPopup2').hasClass('hide')) {
				$('.userAddPopup2 input[name="deptName"]').val(deptName);
			}
		// },
		// onDoubleClick: function(deptName, deptId, companyId) {
		// 	// 더블클릭 시에만 사용자 목록 갱신
		// 	selectedDeptId = deptId;
		// 	selectedDeptName = deptName;
		// 	console.log('더블클릭 선택된 부서:', {
		// 		deptName: deptName,
		// 		deptId: deptId,
		// 		companyId: companyId
		// 	});
			
			// userTable이 초기화되었는지 확인
			if (userTable) {
				loadUserTable();
			} else {
				console.error('userTable이 초기화되지 않았습니다.');
			}
		}
	});

	tree.render(treeData);
	deptTree = tree;
}

function modifyDepartment() {
	const deptId = $(".departModifyPopup input[name='deptId']").val();
	const deptName = $(".departModifyPopup input[name='deptName']").val();

	if (!deptName) {
		alert("부서명을 입력해주세요.");
		return;
	}
	
	$.ajax({
		url: '/primx/fun/dpt/update.do',
		type: 'POST',
		data: {
			deptId: deptId,
			deptName: deptName
		},
		success: function(response) {
			if (response.success) {
				alert(response.message);
				$(".departModifyPopup").addClass("hide");
				// 트리 새로고침
				loadDeptTree();
			} else {
				// 서버에서 전달된 에러 메시지를 그대로 표시
				alert(response.message);
			}
		},
		error: function(xhr, status, error) {
			console.error('부서 수정 실패:', error);
			alert("부서 수정 중 오류가 발생했습니다.");
		}
	});
}

function openDepartModifyPopup() {
	// 현재 선택된 부서 정보 로깅
	// console.log('부서 수정 시도 - 선택된 부서 정보:', {
	// 	selectedDeptId: selectedDeptId,
	// 	selectedDeptName: selectedDeptName
	// });

	// 선택된 부서가 없는 경우
	if (!selectedDeptId) {
		console.warn('수정할 부서가 선택되지 않음');
		alert("수정할 부서를 선택해주세요.");
		return;
	}

	// // 선택된 부서 정보 조회
	// console.log('부서 정보 조회 요청:', {
	// 	url: '/primx/fun/dpt/details.do',
	// 	deptId: selectedDeptId
	// });

	$.ajax({
		url: '/primx/fun/dpt/details.do',
		type: 'GET',
		data: {
			deptId: selectedDeptId
		},
		success: function(response) {
			// console.log('부서 상세 정보 응답:', response);  // 응답 데이터 로그
			if (response && response.data) {
				// console.log('부서 상세 정보 설정:', response.data);
				// 부서 정보를 팝업 필드에 설정
				$(".departModifyPopup input[name='deptName']").val(response.data.deptName);
				$(".departModifyPopup input[name='parentDeptName']").val(response.data.parentDeptName || '');
				$(".departModifyPopup input[name='parentDeptId']").val(response.data.parentId || '');
				$(".departModifyPopup input[name='companyId']").val(response.data.companyId);
				$(".departModifyPopup input[name='deptId']").val(response.data.deptId);
				
				// 설정된 값 확인 로그
				// console.log('팝업 필드 설정 완료:', {
				// 	deptName: response.data.deptName,
				// 	parentDeptName: response.data.parentDeptName,
				// 	parentDeptId: response.data.parentId,
				// 	companyId: response.data.companyId,
				// 	deptId: response.data.deptId
				// });
				
				// 팝업 표시
				$(".departModifyPopup").removeClass("hide");
			} else {
				console.error('부서 정보 없음:', response);  // 실패 시 상세 로그
				alert(response.message);
			}
		},
		error: function(xhr, status, error) {
			console.error("부서 정보 조회 실패:", {
				status: status,
				error: error,
				response: xhr.responseText
			});
			alert("부서 정보를 불러오는 중 오류가 발생했습니다.");
		}
	});
}

function openDepartDeletePopup() {
	console.log('openDepartDeletePopup 호출');
	console.log('selectedDeptId: ', selectedDeptId);
	console.log('selectedDeptName: ', selectedDeptName);
	// 선택된 부서가 없는 경우
	if (!selectedDeptId || !selectedDeptName) {
		alert("삭제할 부서를 선택해주세요.");
		return;
	}

	// 삭제할 부서명 표시
	$(".departDeletePopup .departNameSpan").text('"' + selectedDeptName + '"');
	// 팝업 표시
	$(".departDeletePopup").removeClass("hide");
}

function initUsePeriodListeners() {

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
        var $expiredDateInput = $(this).closest('.useDate').find('input[name="userRegistrationExpiredDate"]');
        
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

function openResetPasswordPopup() {

	const selectedUsers = getSelectedUserIds();
	console.log('selectedUsers: ', selectedUsers);
	const selectedUserNames = getSelectedUserNames();
	if (selectedUsers.length === 0) {
		alert('초기화할 사용자를 선택해주세요.');
		$('.passwordResetPopup').addClass('hide');
		return;
	}
	else{
		$('#userNameToReset').text(selectedUserNames.join(', ') + ' 사용자의 비밀번호를 초기화 하시겠습니까?');
		$('.passwordResetPopup').removeClass('hide');
	}
}

function resetPassword() {
	const selectedUsers = getSelectedUserIds();
	selectedUsers.forEach(userId => {
		$.ajax({
			url: '/primx/fun/usr/resetPassword.do',
			type: 'POST',
			data: { emplyrId: userId },
			success: function(response) {
				console.log('비밀번호 초기화 응답:', response);
				loadUserTable();
				alert(response.message);
			},
			error: function(xhr, status, error) {
				console.error('비밀번호 초기화 실패:', {
					status: status,
					error: error,
					response: xhr.responseText
				});
				alert("비밀번호 초기화 중 오류가 발생했습니다.");
			}	
		});
	});
	$('.passwordResetPopup').addClass('hide');
}

function openUserDeletePopup() {
	const selectedUsers = getSelectedUserIds();
	const selectedUserNames = getSelectedUserNames();
	if (selectedUsers.length === 0) {
		alert('삭제할 사용자를 선택해주세요.');
		$('.userDeletePopup').addClass('hide');
		return;
	} else {
		// 선택된 사용자 이름을 팝업에 표시
		$('#userNameToDelete').text('"' + selectedUserNames.join(', ') + '"');
		$('.userDeletePopup').removeClass('hide');
	}
}

function saveUserAdd() {
	const idCheckFlag = $('input[name="idCheckFlag"]').val();
	const checkedUserId = $('input[name="checkedUserId"]').val();
	const emplyrId = $('.userAddPopup input[name="userId"]').val().trim();
	const userName = $('.userAddPopup input[name="userName"]').val().trim();
	const email = $('.userAddPopup input[name="email"]').val().trim();
	const mailCheckFlag = $('input[name="mailCheckFlag"]').val();
	const checkedMail = $('input[name="checkedMail"]').val();
	const deptName = $('.userAddPopup input[name="deptName"]').val().trim();
	const deptId = $('.userAddPopup input[name="deptId"]').val();
	const companyId = $('.userAddPopup input[name="companyId"]').val();
	const userRole = $('.userAddPopup select[name="userRole"]').val();
	const usePeriod = $('.userAddPopup select[name="userRegistrationPeriod"]').val();
	const expiredDate = $('.userAddPopup input[name="userRegistrationExpiredDate"]').val();
	const ipPlusChecked = $('.userAddPopup .ipPlus').is(':checked');
	const ipInputs = $('.userAddPopup .ipTable input[type="text"]');
	
	// 데이터 검증
	if (!emplyrId) {
		alert('사용자 ID를 입력해주세요.');
		return;
	}
	if (idCheckFlag === 'N') {
		alert('이미 존재하는 ID입니다.');
		return;
	}
	if (checkedUserId !== emplyrId) {
		alert('사용자 ID가 변경되었습니다. 다시 확인해주세요.');
		return;
	}
	if (!userName) {
		alert('사용자명을 입력해주세요.');
		return;
	}
	if (!email) {
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
	if (!deptName) {
		alert('부서를 선택해주세요.');
		return;
	}
	if (!userRole) {
		alert('사용자 레벨을 선택해주세요.');
		return;
	}

	// usePeriod가 1인 경우 expiredDate 값 확인
	if (usePeriod === "1" && !expiredDate) {
		alert('종료일을 선택해주세요.'); // 알림 띄우기
		return;
	}

	// IP 리스트 생성
	let ipList = [];
    const ipRows = $('.userAddPopup .ipTable tbody tr');
	if (ipPlusChecked) {
		ipRows.each(function() {
			// 이 행에서만 IP 입력 4칸을 모아서 하나의 IP로 만든다
			let ipParts = [];
			$(this).find('input[type="text"]').each(function() {
				const ipValue = $(this).val().trim();
	
				// 공백이거나 숫자 형식이 아닌 경우, 또는 0~255 범위를 벗어나는 경우
				if (!/^\d+$/.test(ipValue) || parseInt(ipValue, 10) < 0 || parseInt(ipValue, 10) > 255) {
					alert("IP 값은 0~255 범위의 숫자만 입력 가능합니다.");
					return; // 현재 행의 each 반복문 탈출
				}
	
				if (ipValue) {
					ipParts.push(ipValue);
				}
			});
	
			// 옥텟이 정확히 4개일 때만 유효한 IP로 간주
			if (ipParts.length === 4) {
				ipList.push({
					allowedIp: ipParts.join('.')
				});
			}
		});
	}

	// 사용자 등록 데이터 준비
	const userRegistrationData = {
		emplyrId: emplyrId,
		userName: userName,
		email: email,
		deptId: deptId,
		companyId: companyId,
		userRole: userRole,
		usePeriod: usePeriod,
		expiredDate: expiredDate,
		ipList: ipList 
	};
	console.log('데이터 출력:', {
		emplyrId: emplyrId,
		userName: userName,
		email: email,
		deptId: deptId,
		companyId: companyId,
		userRole: userRole,
		usePeriod: usePeriod,
		expiredDate: expiredDate,
		ipList: ipList 
	});

	// 사용자 등록 요청
	$.ajax({
		url: '/primx/fun/usr/registerUser.do',
		type: 'POST',
		contentType: 'application/json',
		data: JSON.stringify(userRegistrationData),
		success: function(response) {
			if (response.success) {
				console.log('사용자 등록 성공:', response);
				alert(response.message);
				loadUserTable();
				$('.userAddPopup').addClass('hide'); // 팝업 닫기
				// 추가적인 작업 (예: 사용자 목록 갱신 등)
			} else {
				console.log('사용자 등록 실패:', response);
				alert(response.message);
			}
		},
		error: function(xhr, status, error) {
			console.error("사용자 등록 실패:", {
				status: status,
				error: error,
				response: xhr.responseText
			});
			alert("사용자 등록 중 오류가 발생했습니다.");
		}
	});
}

function addIp() {
	// 현재 팝업에 따라 IP 테이블의 tbody 선택
	const ipTableBody = $('.userAddPopup .ipTable tbody, .userModifyPopup .ipTable tbody'); // 두 팝업의 tbody 선택
	const newRow = `
		<tr>
			<td><input type="checkbox" name="chk" id=""></td>
			<td>
				<input type="text" class="txtBox minInput" maxlength="3"><span class="portDot">.</span>
				<input type="text" class="txtBox minInput" maxlength="3"><span class="portDot">.</span>
				<input type="text" class="txtBox minInput" maxlength="3"><span class="portDot">.</span>
				<input type="text" class="txtBox minInput" maxlength="3">
			</td>
		</tr>
	`;
	ipTableBody.append(newRow); // 새로운 행 추가
}

function removeIp() {
	// 현재 팝업에 따라 IP 테이블의 tbody 선택
	const ipTableBody = $('.userAddPopup .ipTable tbody, .userModifyPopup .ipTable tbody'); // 두 팝업의 tbody 선택
	
    // 선택된 IP 입력 행 제거
    const checkedRows = ipTableBody.find('input[name="chk"]:checked').closest('tr');
    if (checkedRows.length > 0) {
        checkedRows.remove(); // 체크된 행 제거
    } else {
        alert('삭제할 IP를 선택해주세요.'); // 선택된 IP가 없을 경우 경고
    }
	
}

function deleteUser() {
	const selectedUsers = getSelectedUserIds();
	selectedUsers.forEach(userId => {
		$.ajax({
			url: '/primx/fun/usr/deleteUser.do', // 사용자 삭제 요청 URL
			type: 'POST',
			data: { emplyrId: userId },
			success: function(response) {
				console.log('사용자 삭제 응답:', response);
				alert(response.message); // 컨트롤러에서 설정한 메시지 출력
				// 사용자 목록 갱신
				loadUserTable();
			},
			error: function(xhr, status, error) {
				console.error('사용자 삭제 실패:', {
					status: status,
					error: error,
					response: xhr.responseText
				});
				alert("사용자 삭제 중 오류가 발생했습니다.");
			}	
		});
	});
	$('.userDeletePopup').addClass('hide'); // 팝업 닫기
}

// 사용자 정보를 로드하는 함수
async function loadUserDetails(emplyrId) {

	resetUserForm('edit');

	// 롤 권한 로드
	var roleData = await loadRoleInfo(2);

	// searchArea_p_Popup userSearchPopup의 userSelectBox 권한 정보 추가
	$('.userModifyPopup select[name="userRole"]').empty();
	for(var i = 0; i < roleData.data.length; i++) {
		console.log('roleData[i]: ', roleData.data[i]);
		$('.userModifyPopup select[name="userRole"]').append('<option value="' + roleData.data[i].authorCode + '">' + roleData.data[i].authorDescription + '</option>');
	}

	$.ajax({
		url: '/primx/fun/usr/details.do',
		type: 'GET',
		data: { emplyrId: emplyrId },
		success: function(response) {
			if (response.success) {
				const userDetails = response.data;

				// 사용자 정보 설정
				$('.userModifyPopup input[name="userId"]').val(userDetails.emplyrId);
				$('.userModifyPopup input[name="userName"]').val(userDetails.userName);
				$('.userModifyPopup input[name="email"]').val(userDetails.email);
				$('.userModifyPopup input[name="checkedMail"]').val(userDetails.email);
				$('.userModifyPopup input[name="mailCheckFlag"]').val('Y');
				$('.userModifyPopup input[name="deptName"]').val(userDetails.deptName);
				$('.userModifyPopup select[name="userRole"]').val(userDetails.userRole);

				selectedDeptId = userDetails.deptId;
				selectedcompanyId = userDetails.companyId;
				selectedDeptName = userDetails.deptName;
				
				// 사용기간 설정
				setExpiredDate(userDetails.expiredDate);

				console.log(userDetails);

				// IP 정보 설정
				const ipList = userDetails.ipList; // IP 리스트를 response.data에서 가져옴
				const ipTableBody = $('.ipTable tbody');
				ipTableBody.empty(); // 기존 IP 정보 초기화
				$('#ipCheckbox').prop('checked', false);
				ipList.forEach(ip => {
					const ipParts = ip.allowedIp.split('.'); // IP 주소를 . 기준으로 나누기
					ipTableBody.append(`
						<tr>
							<td><input type="checkbox" name="chk" id=""></td>
							<td>
								<input type="text" class="txtBox minInput" value="${ipParts[0] || ''}" maxlength="3">
								<span class="portDot">.</span>
								<input type="text" class="txtBox minInput" value="${ipParts[1] || ''}" maxlength="3">
								<span class="portDot">.</span>
								<input type="text" class="txtBox minInput" value="${ipParts[2] || ''}" maxlength="3">
								<span class="portDot">.</span>
								<input type="text" class="txtBox minInput" value="${ipParts[3] || ''}" maxlength="3">
							</td>
						</tr>
					`);
				});
				if(ipList.length > 0) {
					$('#ipCheckbox').prop('checked', true);
				}
				changeModifyAreaBox();
			} else {
				alert(response.message);
			}
		},
		error: function(xhr, status, error) {
			console.error('사용자 정보 로드 실패:', error);
			alert("사용자 정보를 불러오는 중 오류가 발생했습니다.");
		}
	});
	
	$('.userModifyPopup').removeClass('hide'); // 수정 팝업 열기
}

function setExpiredDate(dateStr) {
	if (!dateStr) return '';
	const dateParts = dateStr.split('-');
	const year = dateParts[0];
	const month = dateParts[1];
	const day = dateParts[2].substring(0, 2);
	
	if (year === '9999') {
		$('.userModifyPopup select[name="userRegistrationPeriod"]').val('0'); // usePeriod를 0으로 설정
		$('.datepicker').attr('disabled', true);
		$('.userModifyPopup input[name="userRegistrationExpiredDate"]').val('9999-12-31'); // 만료일 설정
	}
	else {
		$('.userModifyPopup select[name="userRegistrationPeriod"]').val('1'); // usePeriod를 1로 설정
		$('.datepicker').attr('disabled', false); // datepicker 표시
		$('.userModifyPopup input[name="userRegistrationExpiredDate"]').val(`${year}-${month}-${day}`); // 만료일 설정
	}
}

// 부서 선택 팝업 닫기
function closeDepartSelectPopup() {
	$('.departSelectPopup').addClass('hide');
}

// 부서 삭제 함수
function deleteDepartment() {
	if (!selectedDeptId) {
		alert("삭제할 부서를 선택해주세요.");
		return;
	}

	$.ajax({
		url: '/primx/fun/dpt/delete.do',
		type: 'POST',
		data: {
			deptId: selectedDeptId
		},
		success: function(response) {
			console.log('부서 삭제 응답:', response);
			if (response.success) {
				alert(response.message || "부서가 삭제되었습니다.");
				$(".departDeletePopup").addClass("hide");
				// 트리 새로고침
				loadDeptTree();
			} else {
				alert(response.message || "부서 삭제에 실패했습니다.");
			}
		},
		error: function(xhr, status, error) {
			console.error("부서 삭제 실패:", {
				status: status,
				error: error,
				response: xhr.responseText
			});
			alert("부서 삭제 중 오류가 발생했습니다.");
		}
	});
}


// 부서 등록 팝업 열기
function openDepartAddPopup() {
	resetUserForm('reg');
	// 입력 필드 초기화
	$(".departAddPopup input[name='deptName']").val('');
	// 선택된 부서가 있으면 상위부서로 설정
	if (selectedDeptId) {
		$(".departAddPopup input[name='parentDeptName']").val(selectedDeptName);
		$(".departAddPopup input[name='parentDeptId']").val(selectedDeptId);
	} else {
		$(".departAddPopup input[name='parentDeptName']").val('');
		$(".departAddPopup input[name='parentDeptId']").val('');
	}
	
	$(".departAddPopup").removeClass("hide");
}

// 회사 등록 팝업 열기
function openCompanyAddPopup() {
	$(".companyAddPopup").removeClass("hide");
}

// 회사 코드 조회 팝업 열기
function openCompanyCodeListPopup() {
    $(".companyCodeListPopup").removeClass("hide");

    let companyCodeListDatatable = null;
    companyCodeListDatatable = initDataTables('#companyCodeListDatatable', {
        ajax: {
            url: '/primx/fun/dpt/loadCompanyCodeList.do',
            type: 'GET'
        },
        columns: [
            { data: 'companyName' },        
            { data: 'companyId' }
        ]
    });
}

// 회사 중복 체크
function checkCompanyCode() {
	const companyCode = $("#companyCode").val();
	console.log('companyCode: ', companyCode);

	if(companyCode == "") {
		alert("회사 코드를 입력해주세요.");
		return;
	}

	if(companyCode.length < 5) {
		alert("회사 코드는 5자리 이상 입력해주세요.");
		return;
	}

	// 특수문자 제외
	if(/[^a-zA-Z0-9]/.test(companyCode)) {
		alert("회사 코드는 영문 대소문자와 숫자만 입력 가능합니다.");
		return;
	}

	$.ajax({
		url: '/primx/fun/dpt/checkCompanyCode.do',
		type: 'POST',
		data: { companyCode: companyCode },
		success: function(response) {
			console.log('response: ', response);
			if (response.success) {
				alert("이미 사용중입니다.");
			} else {
				$("#checkedCompanyCode").val(companyCode);
				alert("사용 가능합니다.");
			}
		},
		error: function(xhr, status, error) {
			console.error("회사 중복 체크 실패:", {
				status: status,
				error: error,
				response: xhr.responseText
			});
			alert("회사 중복 체크 중 오류가 발생했습니다.");
		}
	});
}

// 회사 저장
function saveCompany() {
	var deptData = {
		companyId: $("#companyCode").val(),
		deptId: $("#companyCode").val(),
		deptName: $("#companyName").val().trim(),
		insertedByHrYn: 'N'
	};

	if (!deptData.deptName) {
		alert("회사명을 입력해주세요.");
		return;
	}

	if (!deptData.companyId) {
		alert("회사 코드를 입력해주세요.");
		return;
	}

	if($("#checkedCompanyCode").val() != $("#companyCode").val()) {
		alert("회사 코드가 변경되었습니다. 다시 확인해주세요.");
		return;
	}

	$.ajax({
		url: '/primx/fun/dpt/createCompany.do',
		type: 'POST',
		data: deptData,
		success: function(response) {
			console.log('회사 저장 응답:', response);
			if (response.success) {
				alert(response.message || "회사가 등록되었습니다.");
				$(".companyAddPopup").addClass("hide");
				// 트리 새로고침
				loadDeptTree();
			} else {
				alert(response.message || "회사 등록에 실패했습니다.");
			}
		},
		error: function(xhr, status, error) {
			console.error("회사 등록 실패:", {
				status: status,
				error: error,
				response: xhr.responseText
			});
			alert("회사 등록 중 오류가 발생했습니다.");
		}
	});
}


// 부서 저장
function saveDepartment() {
	var deptData = {
		companyId: $(".departAddPopup input[name='companyId']").val(),
		parentId: $(".departAddPopup input[name='deptId']").val(),
		deptName: $(".departAddPopup input[name='chieldDeptName']").val().trim(),
		insertedByHrYn: 'N'  // 수동 등록은 'N'으로 설정
	};

	if (!deptData.deptName) {
		alert("부서명을 입력해주세요.");
		return;
	}

	if (!deptData.companyId) {
		alert("상위 부서를 선택해주세요.");
		return;
	}

	$.ajax({
		url: '/primx/fun/dpt/create.do',
		type: 'POST',
		data: deptData,
		success: function(response) {
			console.log('부서 저장 응답:', response);
			if (response.success) {
				alert(response.message || "부서가 등록되었습니다.");
				$(".departAddPopup").addClass("hide");
				// 트리 새로고침
				loadDeptTree();
			} else {
				alert(response.message || "부서 등록에 실패했습니다.");
			}
		},
		error: function(xhr, status, error) {
			console.error("부서 등록 실패:", {
				status: status,
				error: error,
				response: xhr.responseText
			});
			alert("부서 등록 중 오류가 발생했습니다.");
		}
	});
}

// 부서 선택 완료
function selectDepartment() {
	const container = document.querySelector('.departSelectPopup .departTreeMap');
	const selectDeptName = container.dataset.selectedDeptName;
	const selectDeptId = container.dataset.selectedDeptId;
	const selectCompanyId = container.dataset.selectedCompanyId;
	console.log('selectDeptName: ', selectDeptName);
	console.log('selectDeptId: ', selectDeptId);
	console.log('selectCompanyId: ', selectCompanyId);

	if (!selectDeptName || !selectDeptId) {
		alert("부서를 선택해주세요.");
		return;
	}

	// 선택된 부서 정보를 부서 등록 팝업의 입력 필드에 설정
	$(".departAddPopup input[name='parentDeptName']").val(selectDeptName);
	$(".departAddPopup input[name='parentDeptId']").val(selectDeptId);
	$(".departAddPopup input[name='companyId']").val(selectCompanyId);

	// 부서 선택 팝업 닫기
	closeDepartSelectPopup();
}

// 사용자 정보를 저장하는 함수
function saveUserChanges() {

	const emplyrId = $('.userModifyPopup input[name="userId"]').val().trim();
	const userName = $('.userModifyPopup input[name="userName"]').val().trim();
	const email = $('.userModifyPopup input[name="email"]').val().trim();
	const mailCheckFlag = $('.userModifyPopup input[name="mailCheckFlag"]').val();
	const checkedMail = $('.userModifyPopup input[name="checkedMail"]').val();
	const deptName = $('.userModifyPopup input[name="deptName"]').val().trim();
	const deptId = $('.userModifyPopup input[name="deptId"]').val().trim();
	const companyId = $('.userModifyPopup input[name="companyId"]').val().trim();
	const userRole = $('.userModifyPopup select[name="userRole"]').val();
	const usePeriod = $('.userModifyPopup select[name="userRegistrationPeriod"]').val();
	const expiredDate = $('.userModifyPopup input[name="userRegistrationExpiredDate"]').val();
	const ipInputs = $('.userModifyPopup .ipTable input[type="text"]');

	// 데이터 검증
	if (!emplyrId) {
		alert('사용자 ID를 입력해주세요.');
		return;
	}
	if (!userName) {
		alert('사용자명을 입력해주세요.');
		return;
	}
	if (!email) {
		alert('이메일을 입력해주세요.');
		return;
	}
	if (mailCheckFlag === 'N') {
		alert('이메일 중복 체크를 해주세요.');
		return;
	}
	if (checkedMail !== email) {
		alert('이메일이 변경되었습니다. 다시 확인해주세요.');
		return;
	}
	if (!deptName) {
		alert('부서를 선택해주세요.');
		return;
	}
	if (!userRole) {
		alert('사용자 레벨을 선택해주세요.');
		return;
	}

	// usePeriod가 1인 경우 expiredDate 값 확인
	if (usePeriod === "1" && !expiredDate) {
		alert('종료일을 선택해주세요.'); // 알림 띄우기
		return;
	}

	// IP 리스트 생성
	let ipList = [];
    const ipRows = $('.userModifyPopup .ipTable tbody tr');
    ipRows.each(function() {
        // 이 행에서만 IP 입력 4칸을 모아서 하나의 IP로 만든다
        let ipParts = [];
        $(this).find('input[type="text"]').each(function() {
            const ipValue = $(this).val().trim();

            // 공백이거나 숫자 형식이 아닌 경우, 또는 0~255 범위를 벗어나는 경우
            if (!/^\d+$/.test(ipValue) || parseInt(ipValue, 10) < 0 || parseInt(ipValue, 10) > 255) {
                alert("IP 값은 0~255 범위의 숫자만 입력 가능합니다.");
                return; // 현재 행의 each 반복문 탈출
            }

            if (ipValue) {
                ipParts.push(ipValue);
            }
        });

        // 옥텟이 정확히 4개일 때만 유효한 IP로 간주
        if (ipParts.length === 4) {
            ipList.push({
                allowedIp: ipParts.join('.')
            });
        }
    });



	// 사용자 등록 데이터 준비
	const userRegistrationData = {
		emplyrId: emplyrId,
		userName: userName,
		email: email,
		deptId: deptId,
		companyId: companyId,
		userRole: userRole,
		usePeriod: usePeriod,
		expiredDate: expiredDate,
		ipList: ipList // IP 리스트 추가
	};
	console.log('데이터 출력:', {
		emplyrId: emplyrId,
		userName: userName,
		email: email,
		deptId: deptId,
		companyId: companyId,
		userRole: userRole,
		usePeriod: usePeriod,
		expiredDate: expiredDate,
		ipList: ipList // IP 리스트 추가
	});

	// 사용자 등록 요청
	 $.ajax({
	 	url: '/primx/fun/usr/updateUser.do', // 사용자 정보 업데이트 요청 URL
	 	type: 'POST',
	 	contentType: 'application/json',
	 	data: JSON.stringify(userRegistrationData),
	 	success: function(response) {
	 		if (response.success) {
	 			alert(response.message);
	 			$('.userModifyPopup').addClass('hide');
	 			loadUserTable(); // 사용자 테이블 새로 고침
	 		} else {
	 			alert(response.message);
	 		}
	 	},
	 	error: function(xhr, status, error) {
	 		console.error("사용자 수정 실패:", {
	 			status: status,
	 			error: error,
	 			response: xhr.responseText
	 		});
	 		alert("사용자 수정 중 오류가 발생했습니다.");
	 	}
	 });
}

function resetUserForm(type) {
	if(type == 'reg'){
		$('.userAddPopup input[name="userId"]').val('');
		$('.userAddPopup input[name="userName"]').val('');
		$('.userAddPopup input[name="email"]').val('');
		$('.userAddPopup input[name="deptName"]').val('');
		$('.userAddPopup select[name="userRole"]').val('');
		$('.userAddPopup select[name="userRegistrationPeriod"]').val(0); // 기본값으로 설정
		$('.userAddPopup input[name="userRegistrationExpiredDate"]').val(''); // 만료일 초기화
		$('.userAddPopup .ipTable tbody').empty(); // IP 테이블 초기화
	} else {
		$('.userModifyPopup input[name="userId"]').val('');
		$('.userModifyPopup input[name="userName"]').val('');
		$('.userModifyPopup input[name="email"]').val('');
		$('.userModifyPopup input[name="deptName"]').val('');
		$('.userModifyPopup select[name="userRole"]').val('');
		$('.userModifyPopup select[name="userRegistrationPeriod"]').val(0); // 기본값으로 설정
		$('.userModifyPopup input[name="userRegistrationExpiredDate"]').val(''); // 만료일 초기화
		$('.userModifyPopup .ipTable tbody').empty(); // IP 테이블 초기화
	}
	
	selectedDeptId = null;
	selectedDeptName = null;
	selectedcompanyId = null;
}

async function openUserAddPopup() {
	clearUserForm();
	loadCompanyInfo($('.userAddPopup select[name="companyId"]'));

	const roleData = await loadRoleInfo(1);
	const targetSelectBox = $('.userAddPopup select[name="userRole"]');
	targetSelectBox.empty();
	for(var i = 0; i < roleData.data.length; i++){
		targetSelectBox.append('<option value="'+ roleData.data[i].authorCode +'">'+ roleData.data[i].authorDescription +'</option>');
	}
	$('.userAddPopup').removeClass('hide');
}

function clearUserForm() {
	$('.userAddPopup input[name="userId"]').val('');
	$('.userAddPopup input[name="userName"]').val('');
	$('.userAddPopup input[name="email"]').val('');
	$('.userAddPopup input[name="deptName"]').val('');
	$('.userAddPopup select[name="userRole"]').val('');
	$('.userAddPopup select[name="userRegistrationPeriod"]').val(0); // 기본값으로 설정
	// .ipPlus checkbox 체크 해제
	$('.userAddPopup .ipPlus').prop('checked', false);
	// ipTable hide
	$('.userAddPopup .ipTable').addClass('hide');
	$('.userAddPopup .ipTable tbody').empty(); // IP 테이블 초기화
}

// 검색
function searchDepartmentUser(popupName) {
	
    if(popupName == 'userSearchPopup2') {
		var searchUserType = $('.userSearchPopup2 .search_wrap select[name="userType"]');
		var searchSyncType = $('.userSearchPopup2 .search_wrap select[name="syncType"]');
		selectedDeptName = null;

		if(searchUserType.val() != '' && searchUserType.val() != null) {
			addFilterItem(".departmentUserSettings .userArea .searchBox", "사용자", searchUserType.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "사용자");
		}

		if(searchSyncType.val() != '' && searchSyncType.val() != null) {
			addFilterItem(".departmentUserSettings .userArea .searchBox", "구분", searchSyncType.find('option:selected').text(), popupName);
		}
		else {
			resetSearchValue(popupName, "구분");
		}
		loadUserTable();
		$('.userSearchPopup2').addClass('hide');
	}
	else if(popupName == 'departSearchPopup2') {
		var searchKeyword = $('.departSearchPopup2 .search_wrap input[name="searchKeyword"]');
		if(searchKeyword.val() != '' && searchKeyword.val() != null) {
			addFilterItem(".departmentUserSettings .departmentArea .searchBox", "부서", searchKeyword.val(), popupName);
		}
		else {
			resetSearchValue(popupName, "부서");
		}

		loadDeptTree(searchKeyword.val());
		$('.searchAreaPopup .departSearchPopup2').addClass('hide');
	}

}
 
function removeFilter(element, popupName) {

	// 선택된 필터의 filterTitle을 찾아서 초기화함수 호출
	var filterTitle = $(element).closest('.filterItem').find('.filterTitle').text();
	resetSearchValue(popupName, filterTitle);
	$(element).closest('.filterItem').remove(); // 해당 필터 아이템 제거

    if(popupName == 'userSearchPopup2') {
		loadUserTable();
    }
	else if(popupName == 'departSearchPopup2') {
		loadDeptTree();
    }
}

function resetSearchValue(popupName, filterTitle) {
    console.log('resetSearchValue: ', filterTitle);

    if(popupName == 'userSearchPopup2') {
		if(filterTitle == '사용자') {
			$('.userSearchPopup2 .search_wrap select[name="userType"]').val('');
		}
		else if(filterTitle == '구분') {
			$('.userSearchPopup2 .search_wrap select[name="syncType"]').val('');
		}
	}
	else if(popupName == 'departSearchPopup2') {
		if(filterTitle == '부서') {
			$('.departSearchPopup2 .search_wrap input[name="searchKeyword"]').val('');
		}
	}
	
}

/* function handleDeptSearch() {
    const searchKeyword = $('#departSelectPopupDeptName').val();
    loadDeptTree(searchKeyword);
}

// 부서 검색 엔터키 이벤트 처리
$("#departSelectPopupDeptName").on("keypress", function(event) {
    if(event.keyCode === 13) {
        event.preventDefault();
        handleDeptSearch();
    }
}); */

// 트리 CSV 내보내기
$('#treeCsvExport').on('click', function() {
    $.ajax({
        url: '/primx/fun/dpt/tree.do',
        type: 'POST',
        data: {
            searchKeyword: '',
            parentId: ''
        },
        success: function(response) {
            if (response && response.data) {
                const today = new Date();
                const formattedDate = today.getFullYear() + 
                    ('0' + (today.getMonth() + 1)).slice(-2) + 
                    ('0' + today.getDate()).slice(-2);
                exportTreeToCsv(response.data, '부서트리_' + formattedDate + '.csv');
            }
        },
        error: function(xhr, status, error) {
            console.error("부서 트리 로드 실패:", error);
        }
    });
});

// 트리 데이터 필드명 매핑 함수
function mapTreeDataFields(treeData) {
    return treeData.map(node => {
        const mappedNode = {
            deptName: node.deptNm || node.deptName || '',
            deptId: node.deptCd || node.deptId || '',
            children: node.children && node.children.length > 0 ? 
                     mapTreeDataFields(node.children.sort((a, b) => {
                         // 부서명으로 정렬하여 일관된 순서 유지
                         const nameA = (a.deptNm || a.deptName || '').toLowerCase();
                         const nameB = (b.deptNm || b.deptName || '').toLowerCase();
                         return nameA.localeCompare(nameB);
                     })) : []
        };
        return mappedNode;
    });
}


