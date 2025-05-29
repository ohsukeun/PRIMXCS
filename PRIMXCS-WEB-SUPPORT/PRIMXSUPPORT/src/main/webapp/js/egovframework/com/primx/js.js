var ww = $(window).width();
var hh = $(window).height();

$(window).resize(function(){
	ww = $(window).width();
	hh = $(window).height();
});

/*document.addEventListener("DOMContentLoaded", function() {
    flatpickr("#datepicker", {
        dateFormat: "Y-m-d", // 날짜 형식
        enableTime: false, // 시간 선택 비활성화
        defaultDate: new Date() // 기본값을 오늘 날짜로 설정
    });
	// select 변경 이벤트
    $(".drop").change(function() {
        var selectedValue = $(this).val(); // 선택된 옵션 값

        if (selectedValue === "무기한") {
            // 무기한 선택 시
            $("#datepicker").val("9999-12-31") // 값 설정
                         .addClass("disabled") // 클래스 추가
                         .prop("disabled", true); // disabled 속성 추가
        } else {
            // 기간 지정 선택 시
            var today = new Date();
            var formattedDate = today.getFullYear() + "-" +
                                ("0" + (today.getMonth() + 1)).slice(-2) + "-" +
                                ("0" + today.getDate()).slice(-2);

            $("#datepicker").val(formattedDate) // 오늘 날짜로 변경
                         .removeClass("disabled") // 클래스 제거
                         .prop("disabled", false); // disabled 속성 제거
        }
    });
});*/

$(function(){
    
	/* 패스워드 보이기 */
	$('.pwViewBtn').on('click', function() {
		const $input = $(this).siblings('input.pwInput'); // 클릭한 버튼의 형제 요소인 input을 선택
		$input.toggleClass('on');
		if ($input.hasClass('on')) {
			$input.attr('type', "text");
		} else {
			$input.attr('type', 'password');
		}
	});

	/**** MENU ****/
	$('header .hamberger').click(function(){
		$('nav').toggleClass('on');
	});
    
});

function logoutMain() {
    // 로그아웃 처리 (세션 무효화 등)
    // 로그아웃 API 호출이나 페이지 리디렉션 코드 추가 가능

    // 메뉴 항목 제거 (DOM에서 삭제)
    var menuItems = document.querySelectorAll('.myPageMenu, .downloadMenu, .logoutMenu');
    menuItems.forEach(function (item) {
        item.remove();  // 메뉴 항목을 DOM에서 제거
    });

    // 로그아웃 후 페이지 리디렉션 (혹은 새로고침)
    window.location.href = '/uat/uia/actionLogout.do';  // 로그아웃 처리 후 리디렉션
}

/* 공통 유효성 검증 함수 */
// 전화번호 형식 검증 (010-XXXX-XXXX)
function validatePhoneNumber(phoneNumber) {
    const phonePattern = /^(01[0-9])-([0-9]{3,4})-([0-9]{4})$/;
    return phonePattern.test(phoneNumber);
}

// 패스워드 유효성 검증 (최소 8자, 영문/숫자/특수문자 조합)
function validatePassword(password) {
    const passwordPattern = /^(?=.*[A-Za-z])(?=.*\d)(?=.*[@$!%*#?&])[A-Za-z\d@$!%*#?&]{8,}$/;
    return passwordPattern.test(password);
}




// 공통 패스워드 설정 정보 로드 후 리턴
/* async function loadCommonPasswordSettingInfo() {
	try {
		const response = await $.ajax({
			url: '/primx/fun/sup/loadPasswordSettingInfo.do',
			type: 'GET'
		});
		console.log("loadCommonPasswordSettingInfo: ", response.data);
		return response.data; // 데이터 반환
	} catch (error) {
		alert("오류 발생: " + error);
		return null; // 오류 발생 시 null 반환
	}
}
 */
// 비밀번호 변경 시 공통 패스워드 설정을 검사하여 변경 가능한지 여부 리턴
/**
 * 성공시 0, 최소길이 실패시 1, 대문자 실패시 2, 숫자 실패시 3, 특수문자 실패시 4
 * @param {*} password 
 * @returns 
 */
/* async function checkCommonPassword(password) {
	const commonPassword = await loadCommonPasswordSettingInfo();
	console.log("commonPassword: ", commonPassword);
	if (!commonPassword) {
		console.error("공통 패스워드 설정 정보를 불러오는 데 실패했습니다.");
		return 0; // 오류 코드 반환
	}

	console.log("commonPassword: ", JSON.stringify(commonPassword));
	// 최소길이 검사
	if (password.length < commonPassword.passMinLength) {
		console.log("최소길이 실패");
		return 1;
	}
	
	// 대문자 검사 - 검사 시 대문자가 passCombUppercase 이상 포함되어야 함
	if (commonPassword.passCombUppercase != 0) {
		const uppercaseCount = (password.match(/[A-Z]/g) || []).length;
		if (uppercaseCount < commonPassword.passCombUppercase) {
			console.log("대문자 실패");
			return 2;
		}
	}
	
	// 숫자 검사 - 검사 시 숫자가 passCombDigit 이상 포함되어야 함
	if (commonPassword.passCombDigit != 0) {
		const digitCount = (password.match(/[0-9]/g) || []).length;
		if (digitCount < commonPassword.passCombDigit) {
			console.log("숫자 실패");
			return 3;
		}
	}
	
	// 특수문자 검사 - 검사 시 특수문자가 passCombSymbol 이상 포함되어야 함
	if (commonPassword.passCombSymbol != 0) {
		const symbolCount = (password.match(/[!@#$%^&*]/g) || []).length;
		if (symbolCount < commonPassword.passCombSymbol) {
			console.log("특수문자 실패");
			return 4;
		}
	}
	
	return 0;
}
 */