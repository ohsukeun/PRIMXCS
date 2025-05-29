/* 설정 > 파일전송 Zone 설정 */
var fileZoneTable = null;
function pageInit(){
	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();
	
	loadFileZoneTable(0);

}

function loadFileZoneTable(){

	fileZoneTable = initDataTables('#fileZoneTable', {
		ajax: {
			url: '/primx/fun/sys/fileZone/list.do',
			type: 'GET'
		},
		paging: false,
		lengthChange: false,
		info: false,
		columnDefs: [
			{
				targets: 0,
				className: 'result'	
			}
		],
		columns: [
			// 체크박스, 프로토콜, 존명
            {
                data: null,
                orderable: false,
                className: "text-center",
                render: function(data, type, row, meta) {
                    if (type === "display") {
                        return `<input type="checkbox" class="row-checkbox" />`;
                    }
                    return "";
                }
            },
			{ data: 'proto' },
			{ data: 'zoneNm' }
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
	
	// 체크박스 단일 선택 이벤트
    $('#fileZoneTable tbody').on('change', '.row-checkbox', function() {
        $('.row-checkbox').not(this).prop('checked', false); // 다른 체크박스 해제
        $('#fileZoneTable tbody tr').removeClass('selected'); // 기존 선택된 행 스타일 제거

        if ($(this).prop('checked')) {
            $(this).closest('tr').addClass('selected'); // 선택된 행 스타일 적용
        }
    });
	
	// 데이터 테이블의 행 클릭 이벤트 추가
	$('#fileZoneTable tbody').off('click');
	$('#fileZoneTable tbody').on('click', 'td:not(:first-child)', async function() {
		const data = fileZoneTable.row(this).data(); // 클릭한 행의 데이터 가져오기
		
		openFileTransferZonePopup(data);
	});

	/* 파일전송zone 탭 */
	$(".fileZoneTabWrap .tabTitle li").on("click", function(){
		var fileZoneTabNum = $(".fileZoneTabWrap .tabTitle li").index($(this));//click하는 인덱스
		$(".fileZoneTabWrap .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
		$(".fileZoneTabWrap .tabCont > li:eq(" + (fileZoneTabNum) + ")").addClass("on");
		$(".fileZoneTabWrap .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
		$(this).addClass("on");
	});
}

function openFileTransferZonePopup(data){
	clearPopupData();
	
	console.log('data: ', data);

	$("#zoneId").val(data.zoneId);
	$("#zoneNm").val(data.zoneNm);
	$("#proto").val(data.proto);

	var zoneData = JSON.parse(data.zoneData);
	console.log('zoneData: ', zoneData);

	// zoneData.script 배열 순회
	zoneData.script.forEach(function(script){
		console.log('script: ', script);
		$("#scriptTable tbody").append(addScriptLine(script.usage, script.script));
	});

	// zoneData.input 배열 순회
	zoneData.input.forEach(function(input){
		console.log('input: ', input);
		$("#inputTable tbody").append(addInputLine(input.param, input.value));
	});


	$(".addFileTransferZonePopup").removeClass("hide");
}


function addFileTransferZonePopup(){

	// 팝업창의 데이터를 초기화하는 함수 호출
	clearPopupData();

	$(".addFileTransferZonePopup").removeClass("hide");
}

function clearPopupData(){
	$("#zoneId").val(0);
	$("#zoneNm").val("");
	$("#proto").val("");
	$("#scriptTable tbody").empty();
	$("#inputTable tbody").empty();

	
	// 팝업 내 탭 초기화
	$(".addFileTransferZonePopup .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
	$(".addFileTransferZonePopup .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
	$('.addFileTransferZonePopup .tabTitle li:first-child').addClass("on"); //첫번째 li에 테두리를 추가해라
	$('.addFileTransferZonePopup .tabCont > li:first-child').addClass("on"); //첫번째 탭을 선택해라

}
           

function addScriptPopup(){
	// 스크립트 테이블에 행 추가
	$("#scriptTable tbody").append(addScriptLine('', ''));
}

function addScriptLine(usage, script){
	// 체크박스, 사용, 스크립트
	return "<tr>"
			+ "<td><input type='checkbox' class='checkBox'></td>"
			+ "<td><input type='text' class='txtBox w100Per' value='" + usage + "'></td>"
			+ "<td><input type='text' class='txtBox w100Per_10' value='" + script + "'></td>"
		+ "</tr>";
}

function addInputPopup(){
	// 입력 테이블에 행 추가
	$("#inputTable tbody").append(addInputLine('', ''));
}

function addInputLine(param, value){
	// 체크박스, 파라미터, 값
	return "<tr>"
			+ "<td><input type='checkbox' class='checkBox'></td>"
			+ "<td><input type='text' class='txtBox w100Per' value='" + param + "'></td>"
			+ "<td><input type='text' class='txtBox w100Per_10' value='" + value + "'></td>"
		+ "</tr>";
}

function deleteScriptPopup(){
	// 선택된 체크박스 row 삭제
	$("#scriptTable tbody").find("tr").each(function(){
		if($(this).find("input[type='checkbox']").is(":checked")){
			$(this).remove();
		}
	});
}

function deleteInputPopup(){
	// 선택된 체크박스 row 삭제
	$("#inputTable tbody").find("tr").each(function(){
		if($(this).find("input[type='checkbox']").is(":checked")){
			$(this).remove();
		}
	});
}

function saveFileZone(){
	// zoneId 가 업데이트 진행

	const zoneId = $("#zoneId").val();
	const zoneNm = $("#zoneNm").val();
	const proto = $("#proto").val();

	if (zoneNm == "") {
		alert("Zone명을 입력해주세요.");
		return;
	}

	if (proto == "") {
		alert("프로토콜을 선택해주세요.");
		return;
	}

	// 스크립트 테이블에 모든 row를 가져와서 json으로 바꾼다
	var scriptTableData = $("#scriptTable tbody").find("tr").map(function(){
		return {
			usage: $(this).find("input[type='text']").eq(0).val(), // 첫번째 값
			script: $(this).find("input[type='text']").eq(1).val() // 두번째 값
		};
	}).get();
	console.log('scriptTableData : ', JSON.stringify(scriptTableData));

	// 입력 테이블에 모든 row를 가져와서 json으로 바꾼다
	var inputTableData = $("#inputTable tbody").find("tr").map(function(){
		return {
			param: $(this).find("input[type='text']").eq(0).val(), // 첫번째 값
			value: $(this).find("input[type='text']").eq(1).val(), // 두번째 값
		};
	}).get();
	console.log('inputTableData : ', JSON.stringify(inputTableData));

    // 전송할 데이터 객체 생성
    const requestData = {
		zoneId: zoneId,
        zoneNm: zoneNm,
        proto: proto,
        zoneData: JSON.stringify({
            script: scriptTableData,
            input: inputTableData
        })
    };
	console.log('requestData : ', JSON.stringify(requestData));
	
	// ajax로 zoneNm, proto, zoneData를 전송
	$.ajax({
		url: '/primx/fun/sys/fileZone/save.do',
        type: 'POST',
        contentType: 'application/json',
        data: JSON.stringify(requestData),
        success: function(response){
            console.log('response : ', response);
            if(response.status === 'success') {
                alert('저장되었습니다.');
                closePopup();
                // 목록 새로고침
                fileZoneTable.ajax.reload();
            } else {
                alert('저장에 실패했습니다.');
            }
        },
        error: function(error){
            console.log('error : ', error);
            alert('저장에 실패했습니다.');
        }
	});

}

function closePopup(){
	
	$(".addFileTransferZonePopup").addClass("hide");
}

function deleteFileZone(){

	// fileZoneTable에 선택된 행이 없으면 메시지 출력
	if(fileZoneTable.rows({ selected: true }).data().length == 0) {
		alert('삭제할 파일전송 Zone을 선택해주세요.');
		return;
	}

	// 한번에 하나만 삭제 가능
	// 선택된 체크박스 row의 zoneId를 가져온다
	var zoneId = fileZoneTable.rows({ selected: true }).data()[0].zoneId;
	
    // 삭제 확인 메시지
    if (!confirm('삭제하시겠습니까?')) {
        return;
    }

	// ajax로 zoneId를 전송
	
	$.ajax({
		url: '/primx/fun/sys/fileZone/delete.do',
		type: 'POST',
		data: {
			zoneId: zoneId
		},
		success: function(response){
			console.log('response : ', response);
			if(response.status === 'success') {
				alert('삭제되었습니다.');
				// 목록 새로고침
				fileZoneTable.ajax.reload();
			} else {
				alert('삭제에 실패했습니다.');
			}
		},
		error: function(error){
			console.log('error : ', error);
			alert('삭제에 실패했습니다.');
		}
	});
	

}
