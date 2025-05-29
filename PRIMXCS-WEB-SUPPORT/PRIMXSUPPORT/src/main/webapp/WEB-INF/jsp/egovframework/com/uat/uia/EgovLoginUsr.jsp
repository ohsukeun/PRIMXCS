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
<title>PRIMX SUPPORT | 로그인</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/base.css'/>"/>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/layout.css'/>"/>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/font.css'/>"/>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/swiper-bundle.min.css'/>"/>
<link rel="icon" href="<c:url value='/images/egovframework/com/primx/favicon.png'/>">

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/jquery-1.7.2.js'/>"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/jquery-ui.min.js'/>"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/js.js' />"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/swiper-bundle.min.js' />"></script>

<script type="text/javaScript" language="javascript">
// 로그인 실행
function actionLogin() {
    if (document.loginForm.id.value == "") {
        alert("<spring:message code='comUatUia.validate.idCheck'/>"); 
    } else if (document.loginForm.password.value == "") {
        alert("<spring:message code='comUatUia.validate.passCheck'/>"); 
    } else {
    	document.getElementById("typeEnt").className = "";
		document.getElementById(objName).className = "on";
    	document.loginForm.userSe.value = "USR";
        document.loginForm.action = "<c:url value='/uat/uia/actionLogin.do'/>";
        document.loginForm.submit();
    }
}

//회원가입 접근
function actionSignUp() {
    location.href = "<c:url value='/primx/fun/sup/actionSignUp.do'/>";
}

</script>

</head>
<body>
<div class="allWrap subPage">
	<!-- Top 영역 -->
	<%@ include file="../../../../egovframework/com/primx/fun/cmm/PrimxUnitTop.jsp" %>
	
    <section class="content">
        <div class="innerWrap loginWrap">
            <form name="loginForm" id="loginForm" method="post" action="<c:url value='/uat/uia/actionLogin.do'/>">
            <input type="hidden" name="userSe" value="USR"/> <!-- 사용자 구분 필드 추가 -->
                <h2 class="mont">LOGIN</h2>
                <ul class="listWrap mont">
                    <li>
                        <input class="txtBox" type="text" name="id"  id="id" placeholder="User ID">
                    </li>
                    <li>
                        <input class="txtBox pwInput" type="password" name="password" placeholder="Password">
                        <button type="button" class="pwViewBtn"></button>
                    </li>
                </ul>
                <div class="buWrap">
                    <button type="submit" class="bu_login" onclick="actionLogin();">
                    	<spring:message code="comUatUia.loginForm.login" />
                    </button>
                    <button class="signUp" type="button" onclick="actionSignUp();"><a href="#none">회원가입</a></button>
                </div>
            </form>
        </div>
    </section>
</div>

</body>
</html>
