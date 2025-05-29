<%@ page pageEncoding="utf-8"%>

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1">
<%-- <link type="text/css" rel="stylesheet" href="<c:url value='/resources/css/common.css' />"> --%>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/DataTables/datatables.css'/>"/>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/jquery-ui.css'/>">
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/base.css'/>"/>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/font.css'/>"/>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/layout.css'/>"/>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/css.css'/>"/>
<link type="text/css" rel="stylesheet" href="<c:url value='/css/egovframework/com/primx/jstree/style.css'/>"/>
<link rel="icon" href="<c:url value='/images/egovframework/com/primx/favicon.png'/>">
<link href="https://hangeul.pstatic.net/hangeul_static/css/nanum-square-neo.css" rel="stylesheet">

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/jquery-1.7.2.js'/>"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/jquery-ui.min.js'/>"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/moment.min.js'/>"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/js.js' />"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/DataTables/datatables.min.js' />"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/DataTables/full_numbers_no_ellipses.js' />"></script>
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<%-- <script type="text/javascript" src="<c:url value='/resources/js/jquery.min.js' />"></script> --%>

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/cmm/customTree.js'/>"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/cmm/common.js' />"></script>

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/i18n.js' />"></script>

<script>
    console.log("1");
    // 세션 체크 함수
    function checkSession() {
        console.log("checkSession 호출");
        return $.ajax({
            url: '<c:url value="/primx/fun/cmm/checkSession.do" />',
            method: 'POST',
            dataType: 'json'
        });
    }

    // i18next 초기화 체크 및 페이지 로드
    function loadMainContent(url) {
        // i18next가 이미 초기화되어 있으면 바로 실행
        if (typeof i18next !== 'undefined') {
            loadPageContent(url);
            return;
        }

        // i18next가 초기화되지 않은 경우에만 재시도 로직 실행
        let retryCount = 0;
        const MAX_RETRIES = 3;
        const RETRY_INTERVAL = 10; // 1초

        loadPageContent(url); // i18next 없이도 기본 동작

        // function tryInitialize() {
        //     if (typeof i18next !== 'undefined') {
        //         loadPageContent(url);
        //         return;
        //     }

        //     retryCount++;
        //     if (retryCount >= MAX_RETRIES) {
        //         console.error('i18next initialization failed after ' + MAX_RETRIES + ' retries');
        //         loadPageContent(url); // i18next 없이도 기본 동작
        //         return;
        //     }

        //     console.warn('Waiting for i18next initialization... (attempt ' + retryCount + ')');
        //     setTimeout(tryInitialize, RETRY_INTERVAL);
        // }

        // tryInitialize();
    }

    // 실제 페이지 컨텐츠를 로드하는 함수
    function loadPageContent(url) {
        checkSession()
            .done(function(response) {
                if (!response.isValid) {
                    alert("세션이 만료되었습니다. 다시 로그인해주세요.1");
                    window.location.href = '<c:url value="/uat/uia/egovLoginUsr.do" />';
                    return;
                }

                $.ajax({
                    url: url,
                    method: 'POST',
                    dataType: 'html'
                })
                .done(function(data) {
                    $('.content').html(data);
                    
                    // pageInit 함수 존재 여부 확인 후 실행
                    if (typeof pageInit === 'function') {
                        pageInit();
                    }
                    
                    // 다른 함수들도 존재 여부 확인 후 실행
                    if (typeof changeModifyAreaBox === 'function') {
                        changeModifyAreaBox();
                    }
                    if (typeof changeAddAreaBox === 'function') {
                        changeAddAreaBox();
                    }

                    // 번역 적용
                    if (typeof applyTranslations === 'function') {
                        applyTranslations();
                    }
                })
                .fail(function(xhr, status, error) {
                    console.error('Error loading content:', error);
                    if (xhr.status === 401 || xhr.status === 403) {
                        alert("접근 권한이 없습니다. 관리자에게 문의해주세요.");
                        window.location.href = '<c:url value="/uat/uia/egovLoginUsr.do" />';
                    } else {
                        $('.content').html('<p data-i18n="error.loadContent">콘텐츠를 불러오는 중 오류가 발생했습니다.</p>');
                    }
                });
            })
            .fail(function(error) {
                console.error('Session check failed:', error);
                alert("세션 인증 실패.");
                window.location.href = '<c:url value="/uat/uia/egovLoginUsr.do" />';
            });
    }

    // 주기적으로 세션 체크 (5분마다)
    setInterval(function() {
        checkSession()
            .done(function(response) {
                if (!response.isValid) {
                    alert("세션이 만료되었습니다. 다시 로그인해주세요.2");
                    window.location.href = '<c:url value="/uat/uia/egovLoginUsr.do" />';
                }
            });
    }, 300000); // 5분 = 300000ms

    // 메뉴 클릭 이벤트
    $(document).on('click', '.menu-link', function (e) {
        e.preventDefault();
        const url = $(this).attr('href');
        loadMainContent(url);
    });
    
    // 초기 콘텐츠 로드
    $(document).ready(function () {
    	const initialURL = '<c:url value="/primx/fun/sys/dashboard/view.do" />';
        /* const initialURL = '<c:url value="/primx/fun/rqt/createRequest/view.do" />'; */
        loadMainContent(initialURL);

        <c:if test="${not empty fn:trim(alertMessage) && alertMessage ne ''}">
            alert("<c:out value='${alertMessage}'/>");
        </c:if>
    });
</script>

