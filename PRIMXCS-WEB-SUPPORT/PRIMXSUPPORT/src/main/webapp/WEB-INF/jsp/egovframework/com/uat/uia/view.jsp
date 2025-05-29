<%@ page contentType="text/html; charset=utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="ui" uri="http://egovframework.gov/ctl/ui"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags"%>

<!DOCTYPE html>
<html lang="ko">
<head>

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>PRIMX SUPPORT</title>
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

</head>

<body>
<div class="allWrap main">
    <!-- Top 영역 -->
	<%@ include file="../../../../egovframework/com/primx/fun/cmm/PrimxUnitTop.jsp" %>
	
    <section class="content mainWrap">
        <!-- Slider main container -->
        <div class="swiper">
            <!-- Additional required wrapper -->
            <div class="swiper-wrapper">
                <!-- Slides -->
                <div class="swiper-slide container zonecentralWrap">
                    <p class="titleWrap">
                        <img src="<c:url value='/images/egovframework/com/primx/zone.png' />" alt="">
                        <span class="mont">SafetyZone Management System</span>
                    </p>
                    <div class="divWrap">
                        <div class="leftWrap">
                            <ul class="itemWrap">
                                <li>영역 암호화</li>
                                <li>커널 레벨 기반 암호화</li>
                                <li>공유 영역 암호화</li>
                                <li>파일 이동을 위한 파일 컨테이너 사용</li>
                                <li>화이트 기반 외부 프로세스 접근 차단</li>
                                <li>개인 인증키 사용</li>
                            </ul>
                            <ul class="itemWrap">
                                <li>효율적인 제품 정책 수립</li>
                                <li>파일 반출을 위한 신청 및 결재</li>
                                <li>제품 사용 이력 및 파일 사용 추적</li>
                                <li>자동 업데이트 지원</li>
                                <li>미 접속 시스템 지동 삭제</li>
                            </ul>
                        </div>
                        <div class="rightWrap">
                            <img src="<c:url value='/images/egovframework/com/primx/zoneImg.svg' />" alt="">
                        </div>
                    </div>
                </div>
                <div class="swiper-slide container orizon">
                    <p class="titleWrap">
                        <img src="<c:url value='/images/egovframework/com/primx/ori.png' />" alt="">
                        <span class="mont">Cloud Management System</span>
                    </p>
                    <div class="divWrap">
                        <div class="leftWrap">
                            <ul class="itemWrap">
                                <li>클라우드 암호화</li>
                                <li>스토리지 서비스 클라우드 최적화</li>
                                <li>클라우드 동기화 폴더 암호화 제공</li>
                                <li>파일 공유를 위한 Share 폴더 지원</li>
                                <li>개인 인증키 사용</li>
                            </ul>
                            <ul class="itemWrap">
                                <li>Microsoft385 Tenant 프로그램 제공</li>
                                <li>WebDav 프로토콜 지원</li>
                                <li>ZONECENTRAL과 개인 인증키 공유 사용</li>
                            </ul>
                        </div>
                        <div class="rightWrap">
                            <img src="<c:url value='/images/egovframework/com/primx/oriImg.svg' />" alt="">
                        </div>
                    </div>
                </div>
                <div class="swiper-slide container cryhod">
                    <p class="titleWrap">
                        <img src="<c:url value='/images/egovframework/com/primx/cry.png' />" alt="">
                        <span class="mont">Disk Management System</span>
                    </p>
                    <div class="divWrap">
                        <div class="leftWrap">
                            <ul class="itemWrap">
                                <li>디스크 암호화</li>
                                <li>다중 디스크 암호화 지원</li>
                                <li>개인 인증키 사용</li>
                            </ul>
                            <ul class="itemWrap">
                                <li>TPM Chip 없이 사용이 가능한 소프트웨어 인증키 사용</li>
                            </ul>
                        </div>
                        <div class="rightWrap">
                            <img src="<c:url value='/images/egovframework/com/primx/cryImg.svg' />" alt="">
                        </div>
                    </div>
                </div>
            </div>
            <!-- If we need pagination -->
            <div class="swiper-pagination"></div>

        </div>
    </section>
</div>
<script>
    
$(function(){
    const swiper = new Swiper('.swiper', {
        // Optional parameters
        direction: 'horizontal',
        loop: true,
        effect: "fade",
        fadeEffect: {
            crossFade: true, // 기존 슬라이드는 완전히 사라지고 새 슬라이드 표시
        },
        autoplay: {
            delay: 5000, // 3초마다 자동 슬라이드
            disableOnInteraction: false, // 사용자 조작 후에도 자동 슬라이드 유지
        },
        // If we need pagination
        pagination: {
            el: '.swiper-pagination',
            clickable: true,
        },
        slidesPerView: 1, // 한 번에 보이는 슬라이드 개수
        speed: 1000, // 슬라이드 전환 속도 (1초)
        scrollbar: {
            el: '.swiper-scrollbar',
        },
    });

});

</script>
</body>

</html>
