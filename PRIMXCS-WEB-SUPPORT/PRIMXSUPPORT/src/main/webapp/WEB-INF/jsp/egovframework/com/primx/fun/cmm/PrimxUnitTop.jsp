<%@ page contentType="text/html; charset=utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags"%>
<header class="container">
    <h1 class="logo">
        <a href="<c:url value='/uat/uia/view.do' />" class="mainLogo menu-link hide">
            <svg version="1.1" id="Calque_1" xmlns="http://www.w3.org/2000/svg" x="0" y="0" viewBox="0 0 1000 224.7" xml:space="preserve"><path d="M398.4 76.4v-.6c0-20.8-6.6-38.4-19-50.7-14.5-14.5-36.2-22.3-64.6-22.3H213.5v221.4h48.6v-71h38.4l47.4 71h56.8L351 145c28-10.6 47.4-32.9 47.4-68.6zm-49.6 2.4c0 18.7-13.6 31.4-37.5 31.4h-49.2V46.8h48.3c23.9 0 38.4 10.9 38.4 31.7v.3zM440 2.7h48.6v221.4H440zm266.7 0l-58.3 93.6-57.9-93.6h-52.6v221.4h47.7V80.3l61.6 93.7h1.2L711 79.4v144.7h48.3V2.7zM0 0h93c54.4 0 87 31.7 87 77.6v.6c0 51.9-41.1 78.8-92.1 78.8H50.1v67.4H0V0zm89.7 113.3c25.1 0 39.6-14.8 39.6-34.1v-.6c0-22-15.7-34.1-40.8-34.1H50.1V113h39.6v.3z" fill="#ffffff"></path><path fill="#d83629" d="M1000 224.7h-57.1L791.3 3.3h56.8z"></path><path fill="#fbaa19" d="M843.2 224.7H788L941.4 3.3h55.3z"></path><linearGradient id="SVGID_1_" gradientUnits="userSpaceOnUse" x1="895.943" y1="137.096" x2="941.552" y2="74.725" gradientTransform="matrix(1 0 0 -1 0 254.196)"><stop offset="0" stop-color="#89133d"></stop><stop offset="1" stop-color="#89133d" stop-opacity="0"></stop></linearGradient><path d="M922.1 111.1l-28.7 41.4 27.8 40.8s-21.4-37.1 4.2-76.7c-1.2-1.8-3.3-5.5-3.3-5.5z" opacity=".71" fill="url(#SVGID_1_)"></path></svg>
        </a>
        <a href="<c:url value='/uat/uia/view.do' />" class="subLogo menu-link hide">
            <svg version="1.1" id="Calque_1" xmlns="http://www.w3.org/2000/svg" x="0" y="0" viewBox="0 0 1000 224.7" xml:space="preserve"><path d="M398.4 76.4v-.6c0-20.8-6.6-38.4-19-50.7-14.5-14.5-36.2-22.3-64.6-22.3H213.5v221.4h48.6v-71h38.4l47.4 71h56.8L351 145c28-10.6 47.4-32.9 47.4-68.6zm-49.6 2.4c0 18.7-13.6 31.4-37.5 31.4h-49.2V46.8h48.3c23.9 0 38.4 10.9 38.4 31.7v.3zM440 2.7h48.6v221.4H440zm266.7 0l-58.3 93.6-57.9-93.6h-52.6v221.4h47.7V80.3l61.6 93.7h1.2L711 79.4v144.7h48.3V2.7zM0 0h93c54.4 0 87 31.7 87 77.6v.6c0 51.9-41.1 78.8-92.1 78.8H50.1v67.4H0V0zm89.7 113.3c25.1 0 39.6-14.8 39.6-34.1v-.6c0-22-15.7-34.1-40.8-34.1H50.1V113h39.6v.3z" fill="#262262"></path><path fill="#d83629" d="M1000 224.7h-57.1L791.3 3.3h56.8z"></path><path fill="#fbaa19" d="M843.2 224.7H788L941.4 3.3h55.3z"></path><linearGradient id="SVGID_1_" gradientUnits="userSpaceOnUse" x1="895.943" y1="137.096" x2="941.552" y2="74.725" gradientTransform="matrix(1 0 0 -1 0 254.196)"><stop offset="0" stop-color="#89133d"></stop><stop offset="1" stop-color="#89133d" stop-opacity="0"></stop></linearGradient><path d="M922.1 111.1l-28.7 41.4 27.8 40.8s-21.4-37.1 4.2-76.7c-1.2-1.8-3.3-5.5-3.3-5.5z" opacity=".71" fill="url(#SVGID_1_)"></path></svg>
        </a>
    </h1>
    <nav>
        <div class="hamberger">
            <span></span>
            <span></span>
        </div>
        <ul class="subList mont">
            <li class="logoutMenu">
                <a href="<c:url value='/uat/uia/egovLoginUsr.do' />" class="menu-link" onclick="logoutMain();">LOGOUT</a>
            </li>
            <li class="loginMenu">
                <a href="<c:url value='/uat/uia/egovLoginUsr.do' />" class="login menu-link">LOGIN</a>
            </li>
            <li class="myPageMenu">
                <a href="<c:url value='/primx/fun/cmm/myPage.do' />" class="menu-link">MY PAGE</a>
            </li>
            <li class="downloadMenu">
                <a href="<c:url value='/primx/fun/cmm/download.do' />" class="menu-link">DOWNLOAD</a>
            </li>
        </ul>
    </nav>
</header>

<script type="text/javascript">
	var isLoggedIn = <%= session.getAttribute("loginVO") != null ? "true" : "false" %>;
	
	var myPageMenu = document.querySelector('.myPageMenu');
	var downloadMenu = document.querySelector('.downloadMenu');
	var logoutMenu = document.querySelector('.logoutMenu');
	var loginMenu = document.querySelector('.loginMenu');
	var menuList = document.querySelector('.subList'); // 메뉴가 포함된 부모 요소
	
	// 로그인 상태일 때
	if (isLoggedIn) {
	    // 'MY PAGE'와 'DOWNLOAD' 메뉴 추가
	    if (!myPageMenu) {
	        var myPageItem = document.createElement('li');
	        myPageItem.classList.add('myPageMenu');
	        myPageItem.innerHTML = '<a href="<c:url value="/primx/fun/cmm/myPage.do" />" class="menu-link">MY PAGE</a>';
	        menuList.appendChild(myPageItem);
	    }
	    if (!downloadMenu) {
	        var downloadItem = document.createElement('li');
	        downloadItem.classList.add('downloadMenu');
	        downloadItem.innerHTML = '<a href="<c:url value="/primx/fun/cmm/download.do" />" class="menu-link">DOWNLOAD</a>';
	        menuList.appendChild(downloadItem);
	    }
	
	    // 로그인 상태에서는 LOGOUT 메뉴 추가, LOGIN 메뉴 제거
	    if (loginMenu) {
	        loginMenu.remove();
	    }
	    if (!logoutMenu) {
	        var logoutItem = document.createElement('li');
	        logoutItem.classList.add('logoutMenu');
	        logoutItem.innerHTML = '<a href="<c:url value="/uat/uia/egovLoginUsr.do" />" class="menu-link" onclick="logoutMain();">LOGOUT</a>';
	        menuList.appendChild(logoutItem);
	    }
	} 
	// 로그아웃 상태일 때
	else {
	    // 'MY PAGE'와 'DOWNLOAD' 메뉴 제거
	    if (myPageMenu) {
	        myPageMenu.remove();
	    }
	    if (downloadMenu) {
	        downloadMenu.remove();
	    }
	
	    // 로그아웃 상태에서는 LOGIN 메뉴 추가, LOGOUT 메뉴 제거
	    if (logoutMenu) {
	        logoutMenu.remove();
	    }
	    if (!loginMenu) {
	        var loginItem = document.createElement('li');
	        loginItem.classList.add('loginMenu');
	        loginItem.innerHTML = '<a href="<c:url value="/uat/uia/egovLoginUsr.do" />" class="login menu-link">LOGIN</a>';
	        menuList.appendChild(loginItem);
	    }
	}
</script>
