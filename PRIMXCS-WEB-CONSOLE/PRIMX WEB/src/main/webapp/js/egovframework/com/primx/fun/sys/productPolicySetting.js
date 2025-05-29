let dataTable = null;
let originalPolicyData = []; // 원본 정책 데이터 저장용 배열

/* 설정 > 제품 기본 정책 설정 */
function pageInit(){
	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();

	/* input file 커스텀 */
	inputFileCustom();
	
	loadProductList();
	
	/* dataTables */
	dataTable = initDataTables('#policyScriptTable');
	
	//initializeDataTable(); // 클라이언트 프로그램 목록 로드

	loadDefaultPolicy();
	
	/* datepicker */
	datePicker();
	
	// 제품 선택 시 데이터 테이블 업데이트
	$('#productSelect').change(function() {
		initializeDataTable();
	});
}


function loadProductList() {
	$.ajax({
		url: '/primx/fun/sys/product/list.do',
		type: 'GET',
		success: function(response) {
			// 제품 목록을 드롭다운에 추가
			response.data.forEach(function(product) {
				$('#productSelect').append(`<option value="${product}">${product}</option>`);
			});

			initializeDataTable();
		},
		error: function(xhr, status, error) {
			console.error('제품 목록 로드 실패:', error);
		}
	});
}

function initializeDataTable() {
	// 데이터 테이블 초기화
	dataTable = initDataTables('#policyScriptTable', {
		processing: true,
		serverSide: true,
		ajax: {
			url: '/primx/fun/sys/policy/script/list.do', // 정책 스크립트 목록을 가져오는 URL
			type: 'POST',
			data: { productName: $('#productSelect').val() }
		},
		columns: [
			{ data: null, // 번호 열
				render: function(data, type, row, meta) {
					return meta.row + meta.settings._iDisplayStart + 1; // 번호 자동 생성
				} 
			},
			{ data: 'scriptName' },
			{ data: 'fileOriginalName'},
			{ data: null, 
				render: function(data, type, row, meta) {
					// console.log('현재 행 데이터:', {
					// 	rowIndex: meta.row,
					// 	scriptId: row.scriptId,
					// 	scriptName: row.scriptName
					// });
					return `
					<div class="w100Per flexWrap">
						<div class="bu_wrap mr10">
							<button class="" type="button" onclick="openFileUploadPopup(${meta.row}, '${row.scriptName}', '${row.fileOriginalName || ''}')">Upload File</button>
						</div>
					</div>
					`;
				} 	
			}
		],
		paging: false, // 페이징 비활성화
		columnDefs: [
			{
				render: DataTable.render.select(),
				targets: 0
			}
		],
		select: {
			style: 'multi',
			selector: 'td:first-child'
		},
		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: null,
			bottomEnd: null,
		},
		drawCallback: function(settings) {
			// 데이터 테이블이 그려진 후 applyTranslations 호출
			applyTranslations();
		}
	});
}


function openFileUploadPopup(rowIndex, scriptName, fileName) {
	// console.log('클릭된 행 정보:', {
	// 	rowIndex: rowIndex,
	// 	scriptName: scriptName,
	// 	fileName: fileName
	// });

	cleanFileUploadPopup();
	
	// 해당 행의 데이터 가져오기
	const rowData = dataTable.row(rowIndex).data();
	// console.log('행 데이터:', rowData);
	
	if (rowData) {
		$('#rowId').val(rowData.id);
		$('#scriptName').val(scriptName);
		$('#fileName').text(fileName);
	} else {
		console.error('해당 행의 데이터를 찾을 수 없습니다:', rowIndex);
	}

	$('.fileUploadPopup').removeClass('hide');
}

function cleanFileUploadPopup() {
	$('#scriptName').val('');
	$('#fileName').text('');
	$('#rowId').val('');
	$('#seqNo').val('');
}

function uploadFile() {
	const rowId = $('#rowId').val();

	// 선택된 파일 정보를 가져온다
	const fileInput = $('.fileUploadPopup').find('input[type="file"]');
	const file = fileInput[0].files[0]; // 선택된 파일

	// 파일 정보를 저장한다
	const formData = new FormData();
	// 파일 정보를 저장할 배열
	const filesData = [];

	filesData.push({
		rowId: rowId,
		fileName: $('#fileName').text(),
		file: file
	});

	console.log('filesData: ', filesData);

	// 파일 정보를 formData에 추가
	filesData.forEach((data) => {
		console.log('data: ', data);
		formData.append('rowIds[]', data.rowId); // 행 번호 추가
		formData.append('fileNames[]', data.fileName); // 파일명 추가
		formData.append('files[]', data.file); // 파일 추가
	});

	formData.append('isGroup', false); // 또는 false로 설정
	
	// 파일 정보를 저장한다
	$.ajax({
		url: '/primx/fun/sys/policy/script/update.do',
		type: 'POST',
		data: formData,
		processData: false, // FormData를 자동으로 변환하지 않도록 설정
		contentType: false, // 기본 contentType을 사용하지 않도록 설정
		success: function(response) {
			if(response.status === 'success') {
				alert('파일 업로드 성공');
				initializeDataTable();
				$('.fileUploadPopup').addClass('hide');
			} else {
				alert('파일 업로드 실패');
			}
		},
		error: function(xhr, status, error) {
			console.error('파일 업로드 실패:', error);
		}
	});
}

// 테이블 행 생성을 처리하는 함수
function createTableRow(polId, item) {
	const row = document.createElement('tr');
	
	// polId에 따라 다른 형식의 행 생성
	switch(polId) {
		case 131:
			// 첫번쨰 td는 item.name에서 확장자를 제거한 name
			const name = item.name.split('.').slice(0, -1).join('.');
			row.innerHTML = `
				<td>${name}</td>
				<td><input type="text" class="txtBox w100Per" value="${item.name}"></td>
				<td>
					<div class="inputFileWrap">
						<input class="inputFile" id="upload${item.name}" type="file" multiple="" style="display: none;">
						<span class="spanFile" id="spanFile${item.name}">${item.val}</span>
						<label class="labelFile" for="upload${item.name}" data-i18n="button.upload">업로드</label>
						<span class="fileName" id="fileName${item.name}"></span>
					</div>
				</td>
			`;
			break;
		default:
			row.innerHTML = `
				<td><input type="text" class="txtBox w100Per" value="${item.name}"></td>
				<td><input type="text" class="txtBox w100Per_10" value="${item.val}"></td>
			`;
			break;
	}
	
	return row;
}

function loadDefaultPolicy() {
	$.ajax({
		url: '/primx/fun/pol/default/list.do',
		type: 'POST',
		success: function(response) {
			if (response && response.resultList) {
				// 원본 데이터 저장
				originalPolicyData = response.resultList;
				
				response.resultList.forEach(function(item) {
					const elementId = 'default' + item.polId;
					const element = document.getElementById(elementId);

					if (element) {
						const parsedData = parsePolData(item.polData);
						if (parsedData) {
							switch(parsedData.type) {
								case 'bool':
									if (element.tagName === 'SELECT') {
										element.value = parsedData.value ? 'Y' : 'N';
									}
									break;
								case 'sz':
								case 'expandsz':
									if (element.tagName === 'INPUT') {
										element.value = parsedData.value;
									}
									break;
								case 'list':
									if (element.tagName === 'TABLE') {
										const tbody = element.querySelector('tbody');
										if (tbody) {
											tbody.innerHTML = '';
											parsedData.value.forEach(listItem => {
												const row = createTableRow(item.polId, listItem);
												tbody.appendChild(row);
											});
										}
									}
									break;
							}
						}
					}
				});
			}
		},
		error: function(xhr, status, error) {
			console.error('정책 목록 로드 실패:', error);
		}
	});
}

// XML 문자열에서 특수 문자를 제거하는 함수
function sanitizeXML(xmlString) {
	// 백슬래시를 임시로 다른 문자로 치환
	xmlString = xmlString.replace(/\\/g, '\\\\');
	
	// 제어 문자 제거 (ASCII 0-31, 127 제외)
	xmlString = xmlString.replace(/[\x00-\x08\x0B\x0C\x0E-\x1F\x7F]/g, '')
		// parsererror 태그와 그 내용 제거
		.replace(/<parsererror[\s\S]*?<\/parsererror>/g, '');
	
	// 임시로 치환한 백슬래시를 원래대로 복원
	return xmlString.replace(/\\\\/g, '\\');
}

// polData를 파싱하여 값을 추출하는 함수
function parsePolData(polData) {
	try {
		// XML 문자열 정제
		const cleanXml = sanitizeXML(polData);
		
		// XML 문자열을 파싱하기 위한 임시 div 생성
		const parser = new DOMParser();
		const xmlDoc = parser.parseFromString(cleanXml, 'text/xml');
		
		// parsererror 체크
		const parseError = xmlDoc.querySelector('parsererror');
		if (parseError) {
			console.error('XML 파싱 에러:', parseError);
			return null;
		}

		const policyElement = xmlDoc.querySelector('policy');
		if (!policyElement) return null;

		// 기본 구조 확인
		const result = {
			type: null,
			value: null
		};

		// bool 타입 체크
		const boolElement = policyElement.querySelector('bool');
		if (boolElement) {
			result.type = 'bool';
			result.value = boolElement.textContent.toLowerCase() === 'true';
			return result;
		}

		// expandsz 타입 체크
		const expandszElement = policyElement.querySelector('expandsz');
		if (expandszElement) {
			result.type = 'expandsz';
			result.value = expandszElement.textContent;
			return result;
		}

		// sz 타입 체크
		const szElement = policyElement.querySelector('sz');
		if (szElement) {
			result.type = 'sz';
			result.value = szElement.textContent;
			return result;
		}

		// list 타입 체크
		const listElements = policyElement.querySelectorAll('list');
		if (listElements.length > 0) {
			result.type = 'list';
			result.value = Array.from(listElements).map(listElement => {
				const nameElement = listElement.querySelector('name');
				const valElement = listElement.querySelector('val');
				
				return {
					name: nameElement ? nameElement.textContent : '',
					val: valElement ? valElement.textContent : ''
				};
			});
			return result;
		}

		return null;
	} catch (error) {
		console.error('XML 파싱 중 에러 발생:', error);
		return null;
	}
}

// 변경된 정책 데이터 저장
function saveModifiedPolicies() {
	
	const modifiedPolicies = [];
	
	// 원본 데이터를 순회하면서 변경사항 확인
	originalPolicyData.forEach(originalItem => {
		const elementId = 'default' + originalItem.polId;
		const element = document.getElementById(elementId);
		
		if (element) {
			try {
				let isModified = false;
				const cleanXml = sanitizeXML(originalItem.polData);
				const parser = new DOMParser();
				const xmlDoc = parser.parseFromString(cleanXml, 'text/xml');
				
				// parsererror 체크
				const parseError = xmlDoc.querySelector('parsererror');
				if (parseError) {
					console.error('XML 파싱 에러:', parseError);
					return;
				}

				const policyElement = xmlDoc.querySelector('policy');
				
				if (element.tagName === 'SELECT') {
					const currentValue = element.value === 'Y';
					const boolElement = policyElement.querySelector('bool');
					if (boolElement && (currentValue !== (boolElement.textContent.toLowerCase() === 'true'))) {
						boolElement.textContent = currentValue ? 'true' : 'false';
						isModified = true;
					}
				} else if (element.tagName === 'INPUT') {
					const currentValue = element.value;
					const szElement = policyElement.querySelector('sz');
					const expandszElement = policyElement.querySelector('expandsz');
					
					if (szElement && szElement.textContent !== currentValue) {
						szElement.textContent = currentValue;
						isModified = true;
					} else if (expandszElement && expandszElement.textContent !== currentValue) {
						expandszElement.textContent = currentValue;
						isModified = true;
					}
				} else if (element.tagName === 'TABLE') {
					const tbody = element.querySelector('tbody');
					if (tbody) {
						const rows = tbody.querySelectorAll('tr');
						const currentLists = Array.from(rows).map(row => {
							if (originalItem.polId === 131) {
								return {
									name: row.querySelector('input[type="text"]').value,
									val: row.querySelector('.spanFile').textContent
								};
							} else {
								return {
									name: row.querySelector('td:first-child input').value,
									val: row.querySelector('td:last-child input').value
								};
							}
						});

						// 기존 list 요소들 제거
						const oldLists = policyElement.querySelectorAll('list');
						oldLists.forEach(list => list.remove());

						// 새로운 list 요소들 추가
						currentLists.forEach(item => {
							const listElement = xmlDoc.createElement('list');
							const nameElement = xmlDoc.createElement('name');
							const valElement = xmlDoc.createElement('val');
							
							nameElement.textContent = item.name;
							valElement.textContent = item.val;
							
							listElement.appendChild(nameElement);
							listElement.appendChild(valElement);
							policyElement.appendChild(listElement);
						});

						// list 요소의 개수나 내용이 변경되었는지 확인
						const originalLists = Array.from(parser.parseFromString(originalItem.polData, 'text/xml').querySelectorAll('list')).map(list => ({
							name: list.querySelector('name')?.textContent || '',
							val: list.querySelector('val')?.textContent || ''
						}));

						isModified = JSON.stringify(originalLists) !== JSON.stringify(currentLists);
					}
				}
				
				if (isModified) {
					// XML을 문자열로 변환할 때 정제된 형태로 변환
					const serializer = new XMLSerializer();
					let xmlString = serializer.serializeToString(xmlDoc);
					
					modifiedPolicies.push({
						polSeq: originalItem.polSeq,
						polCode: originalItem.polCode,
						polId: originalItem.polId,
						polData: xmlString
					});
				}
			} catch (error) {
				console.error('정책 데이터 처리 중 에러 발생:', error);
			}
		}
	});
	
	// 변경된 정책이 있는 경우에만 저장 요청
	if (modifiedPolicies.length > 0) {
		console.log('originalPolicyData:', originalPolicyData);
		console.log('modifiedPolicies:', modifiedPolicies);
		$.ajax({
			url: '/primx/fun/pol/default/save.do',
			type: 'POST',
			data: JSON.stringify(modifiedPolicies),
			contentType: 'application/json',
			success: function(response) {
				console.log('서버 응답:', response);  // 응답 로깅 추가
				if (response && response.status === 'success') {  // success 필드 대신 status 필드 체크
					alert('정책이 성공적으로 저장되었습니다.');
					loadDefaultPolicy(); // 저장 후 데이터 새로고침
				} else {
					alert('정책 저장에 실패했습니다.');
					console.error('저장 실패 응답:', response);
				}
			},
			error: function(xhr, status, error) {
				console.error('정책 저장 실패:', error);
				console.error('상태 코드:', xhr.status);
				console.error('응답 텍스트:', xhr.responseText);
				alert('정책 저장 중 오류가 발생했습니다.');
			}
		});
	}
}

// 정책 데이터를 XML 형식으로 변환
function convertToPolData(data) {
	let xmlContent = '<policy>';
	
	switch(data.type) {
		case 'bool':
			xmlContent += `<bool>${data.value ? 'true' : 'false'}</bool>`;
			break;
		case 'sz':
		case 'expandsz':
			xmlContent += `<${data.type}>${data.value}</${data.type}>`;
			break;
		case 'list':
			data.value.forEach(item => {
				xmlContent += '<list>';
				xmlContent += `<name>${item.name}</name>`;
				xmlContent += `<val>${item.val}</val>`;
				xmlContent += '</list>';
			});
			break;
	}
	
	xmlContent += '</policy>';
	return xmlContent;
}
