<%@ page contentType="text/html; charset=utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="ui" uri="http://egovframework.gov/ctl/ui"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags"%>

<%
 /**
  * @Class Name : EgovLoginUsr.jsp
  * @Description : 사용자 로그인 화면
  * @Modification Information
  * 
  * @수정일      수정자          수정내용
  * ----------   --------       ---------------------------
  * 2024.06.30   개발자명        기존 코드 대체 및 스타일 개선
  *
  * @author 개발자명
  * @since 2024.06.30
  * @version 2.0
  */
%>

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
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/i18n.js' />"></script>

<script type="text/javaScript" language="javascript">
// 로그인 실행
function actionLogin() {
    if (document.loginForm.id.value == "") {
    } else if (document.loginForm.password.value == "") {
    } else {
        document.loginForm.userSe.value = "USR";
        document.loginForm.action = "<c:url value='/uat/uia/actionLogin.do'/>";
        document.loginForm.submit();
    }
}

function validateForm() {
    if (document.loginForm.id.value.trim() === "") {
        alert("<spring:message code='comUatUia.validate.idCheck'/>");
        document.loginForm.id.focus();
        return false;
    }
    if (document.loginForm.password.value.trim() === "") {
        alert("<spring:message code='comUatUia.validate.passCheck'/>");
        document.loginForm.password.focus();
        return false;
    }
    return true;
}

function fnInit() {
    <c:if test="${not empty fn:trim(loginMessage) &&  loginMessage ne ''}">
    alert("<c:out value='${loginMessage}'/>");
    </c:if>
}

</script>
</head>
<body onLoad="fnInit();">
    <form name="loginForm" id="loginForm" method="post" action="<c:url value='/uat/uia/actionLogin.do'/>" onsubmit="return validateForm()">
    <input type="hidden" name="userSe" value="USR"/> <!-- 사용자 구분 필드 추가 -->
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
               <!-- 로그인 폼 -->
               <ul class="loginBox">
                  <li>
                     <input type="text" name="id"  id="id" class="txtBox" data-i18n-placeholder="login.adminid"/>
<!--                      <input type="text" name="id" id="id" class="txtBox" data-i18n-placeholder="login.userid"/> -->
                  </li>
                  <li>
                       <input type="password" name="password" data-i18n-placeholder="login.password" class="txtBox pwInput"/>
                       <button type="button" class="pwViewBtn"></button>
                   </li>
                   <li class="bu_wrap">
                      <button type="submit" class="bu_login" onclick="actionLogin();">
                            <span data-i18n="login.login">로그인</span>
                      </button>
                   </li>
               </ul>
           </section>
        </div>
    </form>
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
