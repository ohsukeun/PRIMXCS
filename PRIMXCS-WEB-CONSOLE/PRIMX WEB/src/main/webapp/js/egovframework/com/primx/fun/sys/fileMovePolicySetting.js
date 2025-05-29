/* 설정 > 이동파일 정책 설정 */
var table = null;
function pageInit(){
	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();
	
	addEventListener();
	
	var fileType = $("#fileType").val();
	loadFileMovePolicySetting(fileType);
}

function addEventListener() {
	console.log("addEventListener");
	// 파일종류가 선택되었을 때 해당 파일 종류의 유효기간, 복호화 횟수, 인증서버 주소로 업데이트
	var fileType = document.getElementById("fileType");
	fileType.addEventListener("change", function() {
		var selectedFileType = fileType.value;
		console.log("selectedFileType: " + selectedFileType);
		loadFileMovePolicySetting(selectedFileType);
	});

	// 유효기간, 복호화 횟수 입력 필드 숫자만 입력 가능하도록 제한
	$("#expiredDate, #decCount").on("input", function() {
		var value = $(this).val();
		if (!/^\d*$/.test(value)) {
			alert($(this).attr("id") === "expiredDate" ? "유효기간은 숫자만 입력 가능합니다." : "복호화 횟수는 숫자만 입력 가능합니다.");
			$(this).val(value.replace(/[^0-9]/g, ''));
		}
	});
}

async function loadFileMovePolicySetting(selectedFileType) {
	if(selectedFileType === "") {
		alert("파일종류를 선택해주세요.");
		return;
	}
	
    var fileExpireDayLimit = await loadServerConfig('FileExpireDayLimit');
    var fileDecryptCountLimit = await loadServerConfig('FileDecryptCountLimit');
    var authServerAddress = await loadServerConfig('AuthServerAddress');

	$("#expiredDate").val(fileExpireDayLimit.valueData);
	$("#decCount").val(fileDecryptCountLimit.valueData);
	$("#certUrl").val(authServerAddress.valueData);

}


function fnSaveFileMovePolicySetting() {
	var fileType = $("#fileType").val();
	var expiredDate = $("#expiredDate").val();
	var decCount = $("#decCount").val();
	var certUrl = $("#certUrl").val();
	
	// 값이 비어있을 경우 기본값 설정
	if(expiredDate === "") {
		expiredDate = 0; // 기본값 설정
	}

	if(decCount === "") {
		decCount = 0; // 기본값 설정
	}

	if(certUrl === "") {
		certUrl = ""; // 기본값 설정
	}
	
	var serverConfigVOList = [
		{
			keyName: 'FileExpireDayLimit',
			valueData: expiredDate
		},
		{
			keyName: 'FileDecryptCountLimit',
			valueData: decCount
		},
		{
			keyName: 'AuthServerAddress',
			valueData: certUrl
		}
	];

	
	// 서버 설정 정보 저장
	$.ajax({
		url: '/primx/fun/sys/saveServerConfig.do',
		type: 'POST',
		data: JSON.stringify(serverConfigVOList),
		contentType: 'application/json',
		success: function(response) {
			if (response.status === "success") {
				alert("이동파일 정책 설정이 완료되었습니다.");
				loadFileMovePolicySetting(fileType);
			} else {
				alert("이동파일 정책 설정에 실패했습니다.");
			}
		},
		error: function(xhr, status, error) {
			alert("오류 발생: " + error);
		}
	});

	// $.ajax({
	// 	url: "/primx/fun/sys/fileMovePolicySetting/saveFileMovePolicySetting.do",
	// 	type: "POST",
	// 	data: JSON.stringify({
	// 		fileType: fileType, 
	// 		expiredDate: expiredDate, 
	// 		decCount: decCount, 
	// 		certUrl: certUrl
	// 	}),
	// 	contentType: 'application/json', // JSON 형식으로 전송
	// 	success: function(data) {
	// 		if(data.status === "success") {
	// 			alert("이동파일 정책 설정이 완료되었습니다.");
	// 			loadFileMovePolicySetting(fileType);
	// 		} else {
	// 			alert("이동파일 정책 설정에 실패했습니다.");
	// 		}
	// 	},
	// 	error: function(xhr, status, error) {
	// 		alert("오류 발생: " + error);
	// 	}
	// });
}
