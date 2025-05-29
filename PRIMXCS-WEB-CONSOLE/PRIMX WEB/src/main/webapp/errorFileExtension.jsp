<%@ page contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ page isErrorPage="true" %>
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>PRIMX | ERROR</title>
<%-- <link href="<c:url value='/css/egovframework/com/com.css' />" rel="stylesheet" type="text/css" /> --%>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/base.css'/>"/>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/font.css'/>"/>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/layout.css'/>"/>

<script language="javascript">
function fncGoAfterErrorPage(){
    history.back(-2);
}
</script>
</head>
<body>
	<section class="error">
		<div class="error_wrap">
			<img src="../../../../../../images/egovframework/com/primx/error.svg" alt="">
			<div class="textBox">
				<p class="error_title">ERROR</p>
				<p class="txt">현재 찾을 수 없는 페이지를 요청 하셨습니다. <br>존재하지 않는 주소를 입력하셨거나, <br>요청하신 페이지의 주소가 변경, 삭제되어 찾을 수 없습니다.</p>
				<button><a href="javascript:fncGoAfterErrorPage();">이전 페이지로 돌아가기 <span>></span></a></button>
			</div>
		</div>
	</section>
<%-- <div style="width: 1000px; margin: 50px auto 50px;">
	<p style="font-size: 18px; color: #000; margin-bottom: 10px; "><img src="<c:url value='/images/egovframework/com/cmm/er_logo.jpg' />" width="379" height="57" /></p>
	<div style="border: 0px solid #666; padding: 20px;">
		<!-- 404 -->
		<p style="color:red; margin-bottom: 8px; ">500 Error</p>

		<div class="boxType1" style="width: 500px;">
			<div class="box">
				<div class="error">
					<p class="title">FILE EXTENSION DENY ERROR</p>
					<p class="cont mb20">HTTP 500 Internal Server error.<br /></p>
					<span class="btn_style1 blue"><a href="javascript:fncGoAfterErrorPage();">이전 페이지</a></span>
				</div>
			</div>
		</div>
	</div>
</div>
 --%>
</body>
</html>