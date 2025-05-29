<%@ page contentType="text/html; charset=utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="ui" uri="http://egovframework.gov/ctl/ui"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags"%>

<!DOCTYPE html>
<html lang="ko">
<head>

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>PRIMX SUPPORT | 회원가입</title>
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


<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/sup/signUp.js' />"></script>

</head>

<body>
<div class="allWrap subPage">
    <!-- Top 영역 -->
	<%@ include file="../cmm/PrimxUnitTop.jsp" %>
	
    <section class="content">
        <div class="innerWrap signUpWrap">
            <form id="signUpForm">
                <h2 class="mont">SIGN UP</h2>
                <ul class="listWrap mont">
                    <li>
                        <span class="tit">아이디</span>
                        <div class="flexWrap">
                            <input class="txtBox" id="userId" type="text" placeholder="사용자 ID를 입력하세요." required>
                            <button class="checkBtn" id="idCheckBtn" type="button">중복 확인</button>
                        </div>
                    </li>
                    <li>
                        <span class="tit">이메일</span>
                        <div class="flexWrap">
                            <input class="txtBox" id="email" type="text" placeholder="이메일을 입력하세요." required>
                            <button class="checkBtn" id="emailCheckBtn" type="button">중복 확인</button>
                        </div>
                    </li>
                    <li>
                        <span class="tit">전화번호</span>
                        <input class="txtBox" id="phoneNumber" type="text" placeholder="'-'을 포함하여 입력하세요.(ex: 010-1234-6789)" required>
                    </li>
                    <li>
                        <span class="tit">사용자명</span>
                        <input class="txtBox" id="userName" type="text" placeholder="사용자명을 입력하세요." required>
                    </li>
                    <li>
                        <span class="tit">회사코드</span>
                        <div class="flexWrap">
                            <input class="txtBox" id="companyCode" type="text" placeholder="회사코드를 정확하게 입력하세요." required>
                            <button class="checkBtn" id="codeCheckBtn" type="button">코드입력 확인</button>
                        </div>
                    </li>
                    <li>
                        <span class="tit">패스워드</span>
                        <div class="mb15">
                            <input class="txtBox pwInput" id="password" type="password" placeholder="패스워드 입력(최소 8자, 영문/숫자/특수문자 조합)" required>
                            <button type="button" class="pwViewBtn"></button>
                        </div>
                        <div>
                            <input class="txtBox pwInput" id="passwordConfirm" type="password" placeholder="패스워드 확인" required>
                            <button type="button" class="pwViewBtn"></button>
                        </div>
                    </li>
                </ul>
                <div class="buWrap">
                    <button class="signUpBtn" id="signUpBtn" type="button">회원가입</button>
                </div>
            </form>
        </div>
    </section>
</div>



</body>

</html>
