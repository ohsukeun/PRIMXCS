/* 설정 > 신청 및 결재 설정 */
var table = null;
function pageInit(){
	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();

	// 체크박스와 텍스트 영역 요소 선택
    const checkbox = document.getElementById('reasonUsedYn');
    const textarea = document.getElementById('noticeReason');

    // 체크박스 상태 변경 시 이벤트 처리
    checkbox.addEventListener('change', () => {
		if (checkbox.checked) {
            textarea.disabled = false; // 체크되었을 때 텍스트 영역 활성화
            textarea.classList.remove('disable'); // disable 클래스 제거
        } else {
            textarea.disabled = true; // 체크 해제 시 텍스트 영역 비활성화
            textarea.classList.add('disable'); // disable 클래스 추가
        }
    });

	initRequestSettingTable();
}

function initRequestSettingTable() {
	initSettingSection();
	table = initDataTables('#requestSettingTable', {
		ajax: {
			url: '/primx/fun/cmm/createRequestSetting/list.do',
			type: 'GET'
		},
		paging: false,  // 페이지네이션 제거
		lengthChange: false, // 페이지당 개수 변경 옵션 제거
		info: false,
 		columnDefs: [
            {
                targets: 0, // 첫 번째 컬럼 (사용/미사용)
                className: 'result' // result 클래스 추가
            }
        ],
		columns: [
			// 사용유무, 구분, 신청명, 결재대상, 수정일 / 여기서 체크박스 수정 불가
			// Y:사용 - 체크, N:미사용 - 미체크
			{ data: function(row) {
                return `<td class="result">${row.useYn === 'Y' ? '<span class="approval">사용</span>' : '<span class="wait">미사용</span>'}</td>`;
            }},
			// ADMIN:관리자, USER:사용자
			{ data: function(row) {
				return row.requestUserRole === 'ADMIN' ? '관리자' : '사용자';
			}},
			//  M:이동파일 생성 신청, A:사용자 등록 신청, C:계정연장신청
			{ data: function(row) {
				return row.requestApprovalCode === 'M' ? '이동파일 생성 신청' : row.requestApprovalCode === 'A' ? '사용자 등록 신청' : '계정연장신청';
			}},
			// U:사용자, A:관리자, AP : 결재자
			{ data: function(row) {
				return row.approvTarget === 'U' ? '사용자' : row.approvTarget === 'A' ? '관리자' : '결재자';
			}},
			{ data: 'modifiedDate', render: function(data) {
				return moment(data).format('YYYY.MM.DD HH:mm:ss');
			}}
		]
	});

	$('#requestSettingTable tbody').off('click');
	$('#requestSettingTable tbody').on('click', 'td:not(:first-child)', function() {
		const row = $(this).closest('tr'); // 클릭된 td의 부모 tr을 찾습니다.
        const selectedRow = table.row(row).data(); // 해당 행의 데이터를 가져옵니다.
		
        // 로그를 통해 선택된 행의 데이터를 출력합니다.
        console.log('클릭된 행:', row);
        console.log('선택된 행 데이터:', selectedRow);
        
        // 기존에 선택된 행에서 'selected' 클래스를 제거합니다.
        $('#requestSettingTable tbody tr').removeClass('selected');

        // 클릭된 행에 'selected' 클래스를 추가합니다.
        row.addClass('selected');
        
		if (selectedRow) {
			setSettingSection(selectedRow);
		} else {
			console.error("선택된 행이 없습니다.");
			// 추가적인 처리 (예: 경고 메시지 표시)
			const noticeReason = document.getElementById('noticeReason');
			noticeReason.value = data.noticeReason;
		}
		// .settingSection에서 hide 클래스 제거
        $('.settingSection').removeClass('hide');
        $('.saveBtn').removeClass('hide');
	});
}

// settingSection에 데이터를 넣어주는 함수
function setSettingSection(data) {
	const noticeSeq = document.getElementById('noticeSeq');
	const useCheckbox = document.getElementById('useCheckbox');
	const noticeContent = document.getElementById('noticeContent');
	const noticeReason = document.getElementById('noticeReason');
	const approvTarget = document.getElementById('approvTarget');
	const reasonUsedYn = document.getElementById('reasonUsedYn');
	const delegatedYn = document.getElementById('delegatedYn');
	const delegatedYnWrap = document.getElementById('delegatedYnWrap');

	noticeSeq.value = data.noticeSeq;
	useCheckbox.checked = data.useYn === 'Y';
	noticeContent.value = data.noticeContent;
	reasonUsedYn.checked = data.reasonUsedYn === 'Y';
	noticeReason.value = data.noticeReason;
	delegatedYn.checked = data.delegatedYn === 'Y';
	if(reasonUsedYn.checked) {
		noticeReason.disabled = false;
		noticeReason.classList.remove('disable');
	}
	else {
		noticeReason.disabled = true;
		noticeReason.classList.add('disable');
	}
	approvTarget.value = data.approvTarget;

	// 전결처리 임시주석
	// if(data.requestApprovalCode === 'M') {
	// 	delegatedYnWrap.classList.remove('hide');
	// }
	// else {
		delegatedYnWrap.classList.add('hide');
	// }

}

// initSettingSection
function initSettingSection() {
	const useCheckbox = document.getElementById('useCheckbox');
	const noticeContent = document.getElementById('noticeContent');
	const noticeReason = document.getElementById('noticeReason');
	const approvTarget = document.getElementById('approvTarget');
	const reasonUsedYn = document.getElementById('reasonUsedYn');
	const delegatedYn = document.getElementById('delegatedYn');
	useCheckbox.checked = false;
	noticeContent.value = '';
	noticeReason.value = '';
	approvTarget.value = 'M';
	reasonUsedYn.checked = false;
	delegatedYn.checked = false;
}


function saveSetting() {
	const noticeSeq = document.getElementById('noticeSeq');
	const useCheckbox = document.getElementById('useCheckbox');
	const noticeContent = document.getElementById('noticeContent');
	const reasonUsedYn = document.getElementById('reasonUsedYn');
	const noticeReason = document.getElementById('noticeReason');
	const approvTarget = document.getElementById('approvTarget');
	const delegatedYn = document.getElementById('delegatedYn');
	const useYn = useCheckbox.checked ? 'Y' : 'N';
	
	// 공지사항 입력 체크
	if (noticeContent.value === '') {
		alert('공지사항을 입력해주세요.');
		return;
	}
	
	// 사유 필수 입력 체크
	if (reasonUsedYn.checked && noticeReason.value === '') {
		alert('사유를 입력해주세요.');
		return;
	}

    // 전송할 데이터 로그
    const dataToSend = {
        noticeSeq: noticeSeq.value,
        useYn: useYn,
        noticeContent: noticeContent.value,
        noticeReason: noticeReason.value,
        reasonUsedYn: reasonUsedYn.checked ? 'Y' : 'N',
        approvTarget: approvTarget.value,
        delegatedYn: delegatedYn.checked ? 'Y' : 'N'
    };
    console.log('전송할 데이터:', dataToSend);

	$.ajax({
		url: '/primx/fun/sys/createRequestSetting/save.do',
		type: 'POST',
		contentType: 'application/json', // JSON 형식으로 전송
		data: JSON.stringify(dataToSend),
		success: function(response) {
			alert('저장되었습니다.');
			initRequestSettingTable();
		},
		error: function(error) {
			alert('저장에 실패했습니다.');
		}
	});
}
