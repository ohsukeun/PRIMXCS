let dataTable = null;

/* 설정 > 업데이트 */
function pageInit(){
	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();

	/* input file 커스텀 */
	inputFileCustom();
	/* dataTables */
	initializeDataTable(); // 클라이언트 프로그램 목록 로드

	/* datepicker */
	datePicker();
}

// 접속 목록 테이블 초기화
function initializeDataTable() {
	// 클라이언트 프로그램 목록
	dataTable = initDataTables('#clientProgramTable');
	// 서버 프로그램 목록
	dataTable = initDataTables('#serverProgramTable');
	// 파일 다운로드 서버
	dataTable = initDataTables('#fileDownloadServerTable');

	getClientProgramList();
	getServerProgramList();
	getFileDownloadServerList();

}



// 클라이언트 프로그램 목록 조회
function getClientProgramList() {
	dataTable = initDataTables('#clientProgramTable', {
		ajax: {
			url: '/primx/fun/sys/clientProgram/list.do',
			type: 'GET',
			dataSrc: '' // 데이터가 배열 형태로 반환될 경우
		},
		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: null,
			bottomEnd: null
		},
		columns: [
			// 번호, 프로그램, OS버전, 파일명, 프로그램 버전, 등록일시, 업로드
			// 번호는 순서대로 부여
			{ data: null, render: function(data, type, row, meta) {
				return meta.row + meta.settings._iDisplayStart + 1;
			} },
			{ data: 'programName' },
			// 64 bit 32 bit 표시	
			{ data: 'processorArchitecture', render: function(data) {
				return data === 0 ? '32 bit' : data === 9 ? '64 bit' : '알 수 없음';
			} },
			{ data: 'fileName' },
			{ data: 'productVersion' },
			{ data: 'updateDate', render: function(data) {
				return moment(data).format('YYYY-MM-DD HH:mm:ss');
			} },
			// 파일 업로드 버튼을 추가한다
			{ data: null, render: function(data, type, row, meta) {
				return createFileUploadClass(meta.row + meta.settings._iDisplayStart, data.seqNo );
			} }
		],
		success: function(response) {
			console.log(response);
			// 🌟 추가된 부분: 동적으로 생성된 HTML 요소에 다국어 적용
			applyTranslations();
		}
	});



	// 파일이 선택되면 선택된 파일의 파일명을 spanFile에 표시
	// $('#clientProgramTable tbody').on('change', 'input[type="file"]', function() {
	// 	const rowData = dataTable.row(this).data(); // 데이터 테이블에서 해당 행의 데이터 가져오기
	// 	// console.log("rowData: ", rowData);
	// 	const rowId = $(this).attr('id').split('upload')[1];
	// 	// console.log("rowId: ", rowId);
	// 	const fileName = $(this).val().split('\\').pop(); // 파일명 추출
	// 	// console.log("fileName: ", fileName);
	// 	// console.log("spanFile: ", `#spanFile${rowId}`);
	// 	$(`#spanFile${rowId}`).text(fileName); // span에 파일명 표시
	// });
}

// 서버 프로그램 목록 조회
function getServerProgramList() {
	dataTable = initDataTables('#serverProgramTable', {
		ajax: {
			url: '/primx/fun/sys/serverProgram/list.do',
			type: 'GET'
		},
		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: null,
			bottomEnd: null
		},
		columns: [
			// 번호, 프로그램, 프로그램 버전, 설치일
			{ data: null, render: function(data, type, row, meta) {
				return meta.row + meta.settings._iDisplayStart + 1;
			} },
			{ data: 'program' },
			{ data: 'version' },
			{ data: 'installDate', render: function(data) {
				return moment(data).format('YYYY-MM-DD HH:mm:ss');
			} }
		],
		success: function(response) {
			console.log(response);

		}
	});
}

// 파일 다운로드 서버 목록 조회
function getFileDownloadServerList() {
	// console.log("getFileDownloadServerList");
	dataTable = initDataTables('#fileDownloadServerTable', {
		ajax: {
			url: '/primx/fun/sys/fileDownloadServer/list.do',
			type: 'GET'
		},
		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: null,
			bottomEnd: null
		},
		columns: [
			// 번호, 서버명, 주소, 추가|삭제
			{ data: null, render: function(data, type, row, meta) {
				return meta.row + meta.settings._iDisplayStart + 1;
			} },
			{ data: 'serverName' },
			{ data: 'serverUrl' },
			{ data: null, render: function(data, type, row) {
				return `
					<div class="bu_wrap mr10">
						<button class="" type="button" onclick="deleteFileDownloadServer(event)">-</button>
					</div>
				`;
			} }
		],
		success: function(response) {
			// console.log("response: ", response);
			if (response.status === 'success') {
				const data = response.data;
				console.log("data: ", data);
			} else {
				console.log("error: ", response.message);
			}
		}
	});
}


// 클라이언트 파일 저장시 파일 정보를 저장한다
function saveClientProgram() {
	// 테이블에서 선택한 row의 seqNo를 가져온다
	const rowId = $('.clientUploadPopup').find('input[name="rowId"]').val();
	const seqNo = $('.clientUploadPopup').find('input[name="seqNo"]').val();
	// 팝업의 값을 가져온다
	const program = $('.clientUploadPopup').find('input[name="program"]').val();
	const osVersion = $('.clientUploadPopup').find('input[name="osVersion"]').val();
	const productVersion = $('.clientUploadPopup').find('input[name="productVersion"]').val();
	// 선택된 파일 정보를 가져온다
	const fileInput = $('.clientUploadPopup').find('input[type="file"]');
	const file = fileInput[0].files[0]; // 선택된 파일

	console.log("rowId: ", rowId);
	console.log("seqNo: ", seqNo);
	console.log("file: ", file);
	console.log("program: ", program);
	console.log("osVersion: ", osVersion);
	console.log("productVersion: ", productVersion);
	
	// 파일이 선택되지 않은 경우 알람을 띄운다
	if(!file) {
		alert('파일을 선택해주세요');
		return;
	}

	// 파일 정보를 저장한다
	const formData = new FormData();
	// 파일 정보를 저장할 배열
	const filesData = [];

	filesData.push({
		rowId: rowId,
		seqNo: seqNo,
		program: program,
		fileName: file.name,
		productVersion: productVersion,
		insertDate: new Date().toISOString(),
		file: file
	});
	
	// 파일 정보를 formData에 추가
	filesData.forEach((data) => {
		formData.append('rowIds[]', data.rowId); // 행 번호 추가
		formData.append('seqNos[]', data.seqNo); // 행 번호 추가
		formData.append('programs[]', data.program); // 프로그램 추가
		formData.append('fileNames[]', data.fileName); // 파일명 추가
		formData.append('productVersion[]', data.productVersion); // 버전 추가
		formData.append('insertDates[]', data.insertDate); // 파일 등록 시간 추가
		formData.append('files[]', data.file); // 파일 추가
	});

	formData.append('isGroup', false); // 또는 false로 설정

	
	// 파일 정보를 저장한다
	$.ajax({
		url: '/primx/fun/sys/clientProgram/update.do', // 컨트롤러의 URL
		type: 'POST',
		data: formData,
		processData: false, // FormData를 자동으로 변환하지 않도록 설정
		contentType: false, // 기본 contentType을 사용하지 않도록 설정
		success: function(response) {
			// status가 success인 경우 알람을 띄운다
			if(response.status === 'success') {
				alert('저장 완료');
				// 성공시 팝업을 닫는다
				closeClientUploadPopup();
				// 성공 시 추가 작업
				getClientProgramList();
			} else {
				alert('저장 중 오류 발생');
			}
			// 로딩 화면 종료
			$('#loadingPopup').addClass('hide');	
		},
		error: function(xhr, status, error) {
			console.error('Error:', error);
			alert('저장 중 오류 발생');
			// 로딩 화면 종료
			$('#loadingPopup').addClass('hide');	
		}
	});

	// 로딩 화면 보여주기
	$('#loadingPopup').removeClass('hide');

}

// 팝업을 닫는다
function closeClientUploadPopup() {
	$('.clientUploadPopup').addClass('hide');
}

// 파일 다운로드 서버 저장	
function saveFileDownloadServer() {

    // 2. 파일 다운로드 서버 목록에서 추가된 row의 서버명, 주소 가져오기
    const fileDownloadServerRows = $('#fileDownloadServerTable tbody tr'); // 테이블의 모든 행을 가져옴
    const newServers = []; // 번호가 없는 행을 저장할 배열

    // ExServerInfoVO의 구조로 전송
    fileDownloadServerRows.each(function() {
        const seqNo = $(this).find('td:eq(0)').text(); // 첫 번째 <td>에서 번호 가져오기
		console.log("seqNo: ", seqNo);
        if (!seqNo) { // 번호가 비어 있는 경우
            const exServerInfoVO = {
                serverName: $(this).find('td:eq(1)').find('input').val(),
                serverUrl: $(this).find('td:eq(2)').find('input').val()
            };
			// 데이터가 전부 있는 경우에만 배열에 추가
			if(exServerInfoVO.serverName && exServerInfoVO.serverUrl) {
				newServers.push(exServerInfoVO); // 배열에 추가
			}
        }
		else {
			console.log("입력 정보가 없습니다");
		}
    });
	
	// console.log("newServers.length: ", newServers.length);
    // 추가된 서버가 있는 경우에만 서버에 전송
    if (newServers.length > 0) {
		
        $.ajax({
            url: '/primx/fun/sys/fileDownloadServer/insert.do',
            type: 'POST',
            data: JSON.stringify(newServers), // 배열을 JSON 문자열로 변환
            contentType: 'application/json', // JSON 형식으로 전송
            success: function(response) {
                // console.log("response: ", response);
                // 파일 다운로드 서버 테이블 새로고침
                getFileDownloadServerList();
            },
            error: function(xhr, status, error) {
                console.error('Error:', error);
                alert('저장 중 오류 발생');
            }
        });
		
    }
}

// 파일 업로드 class 생성
function createFileUploadClass(rowId, seqNo) {
	
	return `
	<div class="w100Per flexWrap">
		<div class="bu_wrap mr10">
			<button class="" type="button" onclick="initProgramUploadPopup(${rowId}, ${seqNo})">Upload File</button>
		</div>
	</div>
	`;
}

// 프로그램 업로드 팝업을 초기화 및 설정한다
function initProgramUploadPopup(rowId, seqNo) {

	// 팝업의 값을 초기화한다
	$('.clientUploadPopup').find('input').val('');

	// 선택된 해당 row의 정보를 가져온다 
	const row = $(`#clientProgramTable tbody tr:eq(${rowId})`);
	// 해당 row의 프로그램, os버전을 가져온다
	const program = row.find('td:eq(1)').text();
	const osVersion = row.find('td:eq(2)').text();
	console.log("program: ", program);
	console.log("osVersion: ", osVersion);

	// 가져온 값을 팝업에 설정한다
	$('.clientUploadPopup').find('input[name="program"]').val(program);
	$('.clientUploadPopup').find('input[name="osVersion"]').val(osVersion);
	$('.clientUploadPopup').find('input[name="rowId"]').val(rowId);
	$('.clientUploadPopup').find('input[name="seqNo"]').val(seqNo);
	$('.clientUploadPopup').removeClass('hide');
}


function addFileDownloadServer() {
	// console.log("addFileDownloadServer");
	// row를 추가한다
	const row = `
	<tr>
		<td></td>
		<td><input type="text" class="txtBox w100Per" value=""></td>
		<td><input type="text" class="txtBox urlInput w100Per" value=""></td>
		<td>
			<div class="bu_wrap mr10">
				<button class="" type="button" onclick="deleteFileDownloadServer(event)">-</button>
			</div>
		</td>
	</tr>	
	`;
	$('#fileDownloadServerTable tbody').append(row);
}

function deleteFileDownloadServer(event) {
	// console.log("deleteFileDownloadServer");

	// 해당 row의 seqNo를 가져온다
    const row = $(event.target).closest('tr'); // 클릭된 버튼의 부모 tr을 찾음
	console.log("row: ", row);
    const table = $("#fileDownloadServerTable").DataTable();
	console.log("table: ", table);
    const rowData = table.row(row).data(); // 해당 행의 데이터를 가져옴
	console.log("rowData: ", rowData);

	
    // rowData가 undefined인 경우
    if (!rowData) {
        row.remove(); // jQuery를 사용하여 행을 삭제
        return;
    }

	// seqNo가 있는 경우에만 삭제한다
	if(rowData.seqNo) {
		console.log("rowData.seqNo: ", rowData.seqNo);
		// seqNo를 가진 row를 삭제한다
		$.ajax({
		url: '/primx/fun/sys/fileDownloadServer/delete.do',
		type: 'POST',
		data: { seqNo: rowData.seqNo },
		success: function(response) {
			// 테이블을 새로고침한다
			getFileDownloadServerList();
		}
		});
	} else {
		console.log("row.remove()");
        row.remove(); // jQuery를 사용하여 행을 삭제
        // DataTable을 업데이트
        table.row(row).remove().draw();
	}
}
