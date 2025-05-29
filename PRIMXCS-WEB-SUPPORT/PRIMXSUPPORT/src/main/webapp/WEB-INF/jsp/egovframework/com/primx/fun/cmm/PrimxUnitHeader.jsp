<%@ page pageEncoding="utf-8"%>

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

<script>
    // 메인 콘텐츠 로드 함수
     function loadMainContent(url) {
        console.log(`Loading content from URL: ${url}`); // 디버깅 로그

        $.ajax({
            url: url,
            method: 'POST',
            dataType: 'html', // 컨트롤러에서 반환된 데이터를 HTML로 처리
            success: function (data) {
                $('.content').html(data);
                pageInit();
                //changeModifyAreaBox();
                //changeAddAreaBox();

                applyTranslations(); // ✅ AJAX로 불러온 페이지에도 번역 적용
            },
            error: function (err) {
                console.error(`Error loading content from ${url}:`, err); // 오류 로그
                document.querySelector('.content').innerHTML = '<p>콘텐츠를 불러오는 중 오류가 발생했습니다.</p>';
            }
        });
    }

    // 메뉴 클릭 이벤트
    $(document).on('click', '.menu-link', function (e) {
        e.preventDefault(); // 기본 동작 방지
        const url = $(this).attr('href'); // 메뉴의 href 값 가져오기
        console.log(`Menu link clicked. Controller URL: ${url}`); // 클릭 이벤트 로그
        loadMainContent(url); // AJAX 호출
    });

    // 초기 콘텐츠 로드
    $(document).ready(function () {       
        const initialURL = '<c:url value="/primx/fun/cmm/myPage.do" />';
        console.log(`Page loaded. Initial content URL: ${initialURL}`); // 초기화 로그
        loadMainContent(initialURL); // 기본 콘텐츠 로드
	});
</script>

