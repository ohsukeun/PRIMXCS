<%@ page pageEncoding="utf-8"%>
<section class="top">
	<div class="top_left">
		<button class="hamberger active"><img src="<c:url value='/images/egovframework/com/primx/hamberger_.svg'/>" alt=""></button>
		<h1 class="logo">
			<a href="<c:url value='/primx/fun/sys/dashboard/view.do' />" class="menu-link" onclick="$('.firstDepthMenu li').removeClass('select open');"><img src="<c:url value='/images/egovframework/com/primx/logo_.png'/>" alt=""></a>
			<%-- <a href="<c:url value='/primx/fun/rqt/createRequest/view.do' />" class="menu-link"><img src="<c:url value='/images/egovframework/com/primx/logo_.png'/>" alt=""></a> --%>
		</h1>
	</div>
	<ul class="top_right">
		<li class="xzedSmartContractSetting hide">
			<div class="bu_wrap">
				<button type="button" class="xzedSmartBtn" onclick="openXZEDSmartContractSettingPopup();" data-i18n="button.xzedSmartContractSetting">XZED 스마트 컨트랙트 설정</button>
			</div>
		</li>
		<li class="timeOutArea"><span class="mm">00</span>분<span class="ss">00</span> 초 후 로그아웃</li>
		<li class="dashBoard hide">
			<a href="<c:url value='/primx/fun/sys/dashboard/view.do' />" class="menu-link" onclick="$('.firstDepthMenu li').removeClass('select open');"><img src="<c:url value='/images/egovframework/com/primx/dashboard-dots_.svg'/>" alt=""></a>
		</li>
		<li class="uiMode">
			<button>
				<img class="moon" src="<c:url value='/images/egovframework/com/primx/moon_.svg'/>" alt="">
				<img class="sunny hide" src="<c:url value='/images/egovframework/com/primx/sunny_.svg'/>" alt="">
			</button>
		</li>
		<li class="language">
			<button><img src="<c:url value='/images/egovframework/com/primx/language_.svg'/>" alt=""></button>
			<div class="dropMenu">
				<span id="langToggle" class="hide"></span>
				<ul>
					<li class="lang"><button type="button" onclick="setLanguage('ko'); return false;">한국어</button></li>
					<li class="lang"><button type="button" onclick="setLanguage('en'); return false;">English</button></li>
				</ul>
			</div>
		</li>
		<li class="alarm">
			<button onclick="initAlarm();"><img src="<c:url value='/images/egovframework/com/primx/bell_.svg'/>" alt=""><span class="newNoti" id="alarmCount">0</span></button>
		</li>
		<li class="setting">
			<button onclick="openSettingPopup();"><img src="<c:url value='/images/egovframework/com/primx/gear_.svg'/>" alt=""></button>
		</li>
		<li class="userLevel">
			<button id="adminLevel"><img src="/images/egovframework/com/primx/admin.svg" alt=""></button>
<!-- 			<button id="adminLevel"><img src="/images/egovframework/com/primx/user.svg" alt=""></button> -->
			<div class="dropMenu">
				<ul>
					<li class="profile"><button type="button" onclick="openProfilePopup();" data-i18n="menu.profile">프로필</button></li>
					<li class="logOut"><button type="button" onclick="logoutMain();" data-i18n="menu.logout">로그아웃</button></li>
				</ul>
			</div>
		</li>
	</ul>
</section>
