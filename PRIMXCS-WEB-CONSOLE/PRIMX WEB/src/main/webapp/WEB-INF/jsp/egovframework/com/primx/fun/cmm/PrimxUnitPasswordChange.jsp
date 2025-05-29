<%@ page contentType="text/html; charset=utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="ui" uri="http://egovframework.gov/ctl/ui"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags"%>

<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>PRIMX | 로그인</title>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/base.css' />">
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/font.css' />">
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/layout.css' />">
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/css.css' />">
<link rel="icon" href="<c:url value='/images/egovframework/com/primx/favicon.png' />">

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/jquery-1.7.2.js' />"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/jquery-ui.min.js'/>"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/moment.min.js'/>"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/i18n.js' />"></script>

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/js.js' />"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/cmm/common.js' />"></script>
<script type="text/javaScript" language="javascript">
// 로그인 실행
async function actionChangePassword() {

	var currentPassword = $("#changePasswordPopupCurrentPassword").val();
	var newPassword = $("#changePasswordPopupNewPassword").val();
	var newPasswordConfirm = $("#changePasswordPopupNewPasswordConfirm").val();

	if (currentPassword === "") {
		alert("현재 비밀번호를 입력해주세요.");
		return;
	}
	if (newPassword === "") {
		alert("신규 비밀번호를 입력해주세요.");
		return;
	}
	if (newPasswordConfirm === "") {
		alert("비밀번호 확인을 입력해주세요.");
		return;
	}
	if (newPassword !== newPasswordConfirm) {
		alert("비밀번호가 일치하지 않습니다.");
		return;
	}

	// 비밀번호 검사
	const passwordCheck = await checkCommonPassword(newPassword);
	console.log("passwordCheck: "+passwordCheck);
	if (passwordCheck !== 0) {
		return;
	}

	$.ajax({
		url: '/primx/fun/usr/updatePassword.do',
		type: 'POST',
		data: {
			emplyrId: "",
			currentPassword: currentPassword,
			newPassword: newPassword
		},
		success: function(response) {
			if (response.success) {
				alert("비밀번호 변경 완료: " + response.message);
				$("#changePasswordPopupCurrentPassword").val("");
				$("#changePasswordPopupNewPassword").val("");
				$("#changePasswordPopupNewPasswordConfirm").val("");
				location.href = "/uat/uia/actionLogin.do";
			} else {
				alert("비밀번호 변경 실패: " + response.message);
			}
		},
		error: function(xhr, status, error) {
			alert("비밀번호 변경 실패: " + error);
		}
	});
}

</script>
</head>
<body>
    <div class="login">
        <section class="login_wrap">
            <h1 class="logo">
                <img src="<c:url value='/images/egovframework/com/primx/logo.png' />" alt="로고">
            </h1>
            <!-- 언어 선택 -->
            <div class="langBox">
                 <button class="currentLang" type="button" id="langToggle">
                     <span data-i18n="login.langToggle"></span>
                 </button>
                 <ul>
                     <li><a href="#" onclick="setLanguage('ko'); return false;">한국어</a></li>
                     <li><a href="#" onclick="setLanguage('en'); return false;">English</a></li>
                 </ul>
             </div>
            <!-- 비밀번호 변경 -->
            <ul class="loginBox">
                    <li data-i18n="login.firstLogin">
                        * 비밀번호 변경 후 사용 가능합니다.
                    </li>
                 <li>
                     <input type="password" data-i18n-placeholder="changePasswordPopup.currentPassword" class="txtBox pwInput" id="changePasswordPopupCurrentPassword"/>
                     <button type="button" class="pwViewBtn" onclick="passwordEyes();"></button>
                 </li>
                 <li>
                     <input type="password" data-i18n-placeholder="changePasswordPopup.newPassword" class="txtBox pwInput" id="changePasswordPopupNewPassword"/>
                     <button type="button" class="pwViewBtn" onclick="passwordEyes();"></button>
                 </li>
                 <li>
                     <input type="password" data-i18n-placeholder="changePasswordPopup.confirmPassword" class="txtBox pwInput" id="changePasswordPopupNewPasswordConfirm"/>
                     <button type="button" class="pwViewBtn" onclick="passwordEyes();"></button>
                 </li>
                 <li class="bu_wrap">
                    <button type="button" class="bu_login" onclick="actionChangePassword();">
                          <span data-i18n="login.changePassword">비밀번호 변경 및 로그인</span>
                    </button>
                 </li>
            </ul>
        </section>
     </div>
</body>
<script type="text/javaScript" language="javascript">
   /* 로그인창 패스워드 보이기 */
   $('.pwViewBtn').on('click', function() {
      const $input = $(this).siblings('input.pwInput'); // 클릭한 버튼의 형제 요소인 input을 선택
      $input.toggleClass('on');
      if ($input.hasClass('on')) {
         $input.attr('type', "text");
      } else {
         $input.attr('type', 'password');
      }
   });

   	/* 로그인 언어선택 */
	$(".login .login_wrap .langBox .currentLang").on("click", function(){
		$('.login .login_wrap .langBox ul').toggleClass('on');
	});
	
	/* 로그인창 언어박스에 현재언어 텍스트 띄우기 */
	$(".login .login_wrap .langBox ul li").on("click", function(){
/*		var currentLang = $(this).text();
		$(".login .login_wrap .langBox .currentLang").text(currentLang);*/
		$('.login .login_wrap .langBox ul').removeClass("on");
	});
</script>
</html>
