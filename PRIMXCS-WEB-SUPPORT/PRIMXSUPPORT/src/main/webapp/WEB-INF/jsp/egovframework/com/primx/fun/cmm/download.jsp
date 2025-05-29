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

<title>PRIMX SUPPORT | 다운로드</title>

</head>

<body>
<div class="allWrap subPage">
	<!-- Top 영역 -->
	<%@ include file="./PrimxUnitTop.jsp" %>
	<!-- Content 영역 -->
	<section class="content">
		<div class="innerWrap downloadWrap">
		    <form action="">
		        <h2 class="mont">DOWNLOAD</h2>
		        <div class="buWrap">
		            <button type="button" class="mb15"><a href="<c:url value='/download/PRIMXAgentSetup_v100b8_106.247.226.210_x64.zip'/>">에이전트</a></button>
		            <button type="button"><a href="<c:url value='/download/PRIMX-CS_UserManual Agent.zip'/>">메뉴얼</a></button>
		        </div>
		    </form>
		</div>
	</section>
</div>

</body>
</html>
<%-- <script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/file/moveFileHistory.js' />"></script> --%>