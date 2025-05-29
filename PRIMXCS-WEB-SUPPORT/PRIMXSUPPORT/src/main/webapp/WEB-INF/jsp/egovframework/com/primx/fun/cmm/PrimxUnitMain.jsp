<%@ page contentType="text/html; charset=utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="ui" uri="http://egovframework.gov/ctl/ui"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags"%>

<!DOCTYPE html>
<html lang="ko">
<head>

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
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

<title>PRIMX SUPPORT | 마이페이지</title>

</head>

<body>
<div class="allWrap container subPage">
	<!-- Top 영역 -->
	<%@ include file="./PrimxUnitTop.jsp" %>
	<!-- Content 영역 -->
	<section class="content">
		<div class="innerWrap myPageWrap">
		    <form action="">
		        <h2 class="mont">
		            <span>MY PAGE</span>
		        </h2>
		        <ul class="listWrap mont">
		            <li>
		                <span class="tit">아이디</span>
		                <div class="flexWrap">
		                    <input class="txtBox disabled" type="text" value="primxuser" readonly disabled>
		                </div>
		            </li>
		            <li>
		                <span class="tit">이메일</span>
		                <div class="flexWrap">
		                    <input class="txtBox disabled" type="text" value="primxuser@primx.com" readonly disabled>
		                </div>
		            </li>
		            <li>
		                <span class="tit">전화번호</span>
		                <input class="txtBox yEdit disabled" type="text" value="010-5369-4982" readonly disabled>
		            </li>
		            <li>
		                <span class="tit">사용자명</span>
		                <input class="txtBox yEdit disabled" type="text" value="김프라이맥스" readonly disabled>
		            </li>
		            <li>
		                <span class="tit">회사코드</span>
		                <input class="txtBox disabled" type="text" value="HR704568" readonly disabled>
		            </li>
		            <li class="pwSection hide">
		                <span class="tit">패스워드</span>
		                <div class="mb15">
		                    <input class="txtBox pwInput yEdit" type="password" placeholder="현재 패스워드 입력">
		                    <button type="button" class="pwViewBtn"></button>
		                </div>
		                <div class="mb15">
		                    <input class="txtBox pwInput yEdit" type="password" placeholder="신규 패스워드 입력">
		                    <button type="button" class="pwViewBtn"></button>
		                </div>
		                <div>
		                    <input class="txtBox pwInput yEdit" type="password" placeholder="패스워드 확인">
		                    <button type="button" class="pwViewBtn"></button>
		                </div>
		            </li>
		            <li class="hide">
		                <span class="tit">계정 만료 기간</span>
		                <div class="flexWrap js">
		                    <input type="text" class="txtBox disabled" value="2025-10-12" readonly disabled>
		                    <button class="accountBtn" type="button"><a href="account.html" target="_blank">계정 연장 신청</a></button>
		                </div>
		            </li>
		        </ul>
		        <div class="buWrap">
		            <button class="editBtn" type="button">수정</button>
		            <button class="saveBtn hide" type="button">저장</button>
		        </div>
		    </form>
		</div>
	</section>
</div>
</body>
</html>
<%-- <script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/file/moveFileHistory.js' />"></script> --%>