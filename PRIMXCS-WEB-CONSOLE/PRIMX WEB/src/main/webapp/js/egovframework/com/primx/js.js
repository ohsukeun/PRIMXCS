var ww = $(window).width();
var hh = $(window).height();

// 호출한 페이지 클래스
var callPageClass = '';

// 미접속 시스템 조회 팝업 테이블
var deleteUnconnectedSystemTable = null;

$(window).resize(function(){
	ww = $(window).width();
	hh = $(window).height();
});

$(function(){
	/* 다크모드 */
	$(".uiMode").click(function() {
		$("html").toggleClass("dark-theme");
		if($("html").hasClass("dark-theme") === true){
			$("img.defaultThem").addClass("hide");
			$("img.darkThem").removeClass("hide");
			$(".uiMode img.moon").addClass("hide");
			$(".uiMode img.sunny").removeClass("hide");
		}else{
			$("img.defaultThem").removeClass("hide");
			$("img.darkThem").addClass("hide");
			$(".uiMode img.moon").removeClass("hide");
			$(".uiMode img.sunny").addClass("hide");
		}
	});
	
	/* 상단 탑 메뉴들 */
	// 버튼 클릭 시 해당 메뉴 열기/닫기
    $(".division > .top .top_right > li.userLevel button, .division > .top .top_right > li.language button").on("click", function (e) {
        e.stopPropagation(); // 이벤트 전파 방지
        const $menu = $(this).siblings(".dropMenu");
        $(".dropMenu").not($menu).removeClass("on"); // 다른 열린 메뉴는 닫기
        $menu.toggleClass("on"); // 현재 메뉴 토글
    });
    /* 프로필 설정 팝업 */
	$(".division > .top .top_right > li.userLevel .dropMenu ul li.profile button").on("click",function(){
		$(".profilePopup").removeClass("hide");
	})
	/* 프로필 설정 팝업 > 수정 */
	$(".profilePopup .modifyProfileBtn").on("click",function(){
		$("input.modifyYes").attr("readonly", false).removeClass("nowrite");
		$(".saveBtn").removeClass("hide");
		$(".modifyProfileBtn").addClass("hide");
	});

	/* 프로필 설정 팝업 > 결재선 등록 */
	$(".registrationApprovalBtn.profileVerBtn").on("click", function(){
		$('.popup.approvalListPopup.profileVerPop').removeClass('hide');
	})
	/* 비밀번호 변경 팝업 */
	$(".profilePopup .changePasswordBtn").on("click",function(){
		$(".changePasswordPopup").removeClass("hide");
	});

    // 문서 클릭 시 모든 메뉴 닫기
    $(document).on("click", function () {
        $(".dropMenu").removeClass("on"); // 모든 메뉴 닫기
    });
    
	/* 왼쪽 메뉴 선택 이벤트 */
	$(".left nav .firstDepthMenu li p").on("click",function(){
		$(this).parent().toggleClass("open");
		
		if($('.division').hasClass('closeMenu')){
			$('.left nav .firstDepthMenu > li').removeClass("open");
        }
	})
	$(".left nav .firstDepthMenu li .secondDepthMenu li").on("click",function(){
		$(".left nav .firstDepthMenu li").removeClass("select");
		$(".left nav .firstDepthMenu li .secondDepthMenu li").removeClass("select");
		$(this).addClass("select");
		$(this).parent().parent().addClass("select");
	})
	
	/* 왼쪽메뉴 하위 뎁스가 없는 경우 */
	$(".left nav .firstDepthMenu li.onlyDepth").on("click",function(){
		$(".left nav .firstDepthMenu li").removeClass("select");
		$(".left nav .firstDepthMenu li.onlyDepth").addClass("select");
		$(this).addClass("select");
	})
	
	/* 왼쪽메뉴 축소이벤트 */
	$(".top .top_left .hamberger").on("click",function(){
		$('.left nav .firstDepthMenu > li').removeClass("open");
		$('.division').toggleClass("closeMenu");
		$('.hamberger').toggleClass("active");

		if($('.division').hasClass('closeMenu')){
			$('.firstDepthMenu > li > p span').addClass("hide");
        }else{
			$('.firstDepthMenu > li > p span').removeClass("hide");
        }
	})	

	/* 화면 확대 축소 */
	$(".per_2").on("click", function(){
		$('html').css('font-size','0.02208333178113vw')
	});
	$(".per_1").on("click", function(){
		$('html').css('font-size','0.03208333178113vw')
	});
	$(".per0").on("click", function(){
		$('html').css('font-size','0.04208333178113vw')
	});
	$(".per1").on("click", function(){
		$('html').css('font-size','0.05208333178113vw')
	});
	$(".per2").on("click", function(){
		$('html').css('font-size','0.06208333178113vw')
	});
	$(".per3").on("click", function(){
		$('html').css('font-size','0.07208333178113vw')
	});


	/* 부서 관리 */
	$(".category_box > ul > li >ul > li a").on("click",function(){
		var it = $(this).parent("li");
		if(!it.hasClass("open") && $(this).hasClass("g21")){
			it.addClass("open");
		}else{
			it.removeClass("open");
		}
		if($(this).hasClass("g23") || $(this).hasClass("g31")){
			$(".category_box .select").removeClass("select");
			$(this).addClass("select");
		}
	});

	$(".team_payer .fullopen").on("click",function(){
		$(".category_box .g2").parent("li").addClass("open");
	});
	
	$(".team_payer .fullclose").on("click",function(){
		$(".category_box .open").removeClass("open");
	});
	
	/* datepicker 상단 설정부분 */
	// Datepicker 초기화
    $(".datePickerSetData").datepicker({
        dateFormat: "yy-mm-dd", // yyyy-mm-dd 형식
        showAnim: "slideDown"  // 애니메이션 효과
    });

    // SelectBox 변경 이벤트 설정
    $(".dateSelectSetData").on("change", function () {
        var $selectBox = $(this); // 현재 선택된 selectBox
        var $parentTd = $selectBox.closest("td"); // 해당 selectBox가 속한 td
        var $startDatePicker = $parentTd.find(".datePickerSetData").eq(0); // 시작일
        var $endDatePicker = $parentTd.find(".datePickerSetData").eq(1); // 종료일

        var selectedValue = $selectBox.val(); // 선택된 값 (3, 6, 9, 12)
        var today = new Date(); // 오늘 날짜
        var startDate = ""; // 시작일 초기화
        var endDate = ""; // 종료일 초기화

        if (selectedValue) {
            // 선택된 값이 존재하는 경우 (기간 지정)
            endDate = $.datepicker.formatDate("yy-mm-dd", today); // 종료일은 오늘 날짜
            var pastDate = new Date(today);
            pastDate.setMonth(today.getMonth() - parseInt(selectedValue)); // 선택된 값만큼 이전
            startDate = $.datepicker.formatDate("yy-mm-dd", pastDate); // 시작일 계산
        } else {
            // "기간 지정" 선택 시 필드 초기화
            startDate = "";
            endDate = "";
        }

        // Datepicker 필드에 값 설정
        $startDatePicker.val(startDate);
        $endDatePicker.val(endDate);
    });

});

/****** section.content의 공통 사용 스크립트 ******/
/****** section.content의 공통 사용 스크립트 ******/ 
/****** section.content의 공통 사용 스크립트 ******/ 

/* 팝업공통 */
function commonPopup(){
	
	/* 로그인창 비밀번호 보이기 */
	$('.pwViewBtn').on('click', function() {
		const $input = $(this).siblings('input.pwInput'); // 클릭한 버튼의 형제 요소인 input을 선택
		$input.toggleClass('on');
		if ($input.hasClass('on')) {
			$input.attr('type', "text");
		} else {
			$input.attr('type', 'password');
		}
	});
    /* tooltip */
	$('table').on('mouseenter', 'td', function() {
	    const cellText = $(this).text(); // 현재 셀의 텍스트 가져오기
	    $(this).attr('title', cellText); // title 속성에 텍스트 설정
	    //$(this).tooltip(); // jQuery UI 툴크 활성화
	});
	
	//팝업 x버튼 클릭시 팝업닫기
	$(document).on("click", ".popup .bu_close_pop",function(){
		$(this).parent().parent(".popup").addClass("hide");
		$("body").css("overflow-y","auto");
	});
	//팝업 뒤 배경 클릭시 팝업 닫힘 (배경클릭닫기 기능 없애려면 html소스에 bgscreen 태그자체를 삭제하기)
	$(document).on("click", ".popup .bgscreen",function(){
		$(this).parent(".popup").addClass("hide");
		$("body").css("overflow-y","auto");
	});
	// 검색 조건 팝업
	$(".searchInnerWrap .searchInput").on("click",function(){
		$(".searchAreaPopup").removeClass("hide");
		$('.searchAreaPopup #searchWord').focus();
		$('.searchAreaPopup #searchText').focus();
		$('.searchAreaPopup #searchKeyword').focus();
	})
	// 검색 조건 팝업(부서)
	$(".departUserSection .departmentArea .searchInput").on("click",function(){
		$(".departSearchPopup2").removeClass("hide");
	})
	// 검색 조건 팝업(사용자)
	$(".departUserSection .userArea .searchInput").on("click",function(){
		$(".userSearchPopup").removeClass("hide");
	})
	/* 상단 설정아이콘 클릭시 팝업 */
	$(".division .top .top_right .setting").on("click",function(){
		$(".topSetPopup").removeClass("hide");
	})
	//tab메뉴
	$(".settabWrap .tabTitle li").on("click", function(){
		var setTabNum = $(".settabWrap .tabTitle li").index($(this));//click하는 인덱스
		$(".settabWrap .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
		$(".settabWrap .tabCont > li:eq(" + (setTabNum) + ")").addClass("on");
		$(".settabWrap .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
		$(this).addClass("on");
	});
}

/* 커스텀 드롭 메뉴 온 오프 */
function dropMenu() {
	/* 드롭 메뉴 온 오프 */
	$(".dropMenu > .selectMenu").on("click",function(){
		$(this).toggleClass("on");
	});
	// 드롭메뉴 li 선택시 메뉴 닫힘
	$(".dropMenu > .selectMenu ~ ul > li").on("click",function(){
		$(".dropMenu > .selectMenu").removeClass("on");
	});
	// 드롭 메뉴 영역 외부 클릭 시 닫기
    $("body").on("click", function (e) {
        if (!$(e.target).closest(".dropMenu").length) {
            $(".dropMenu > .selectMenu").removeClass("on");
        }
    });
}

/* input file 커스텀 */
function inputFileCustom(){
	
	// input file 커스텀 - 파일명 붙이기
	const fileTarget = $('.inputFileWrap input');

	fileTarget.on('change', function () { 
		var files = $(this)[0].files;
		var fileArr = [];
	
		for (var i = 0; i < files.length; i++) {
			fileArr.push(files[i].name);
		}
		
		// 파일명 노출방법1: 배열 값 사이에 공백 추가
		var fileList = fileArr.join(', '); // 배열 값을 쉼표와 공백으로 연결
		$(this).siblings('.spanFile').text(fileList);
	});
}

/* dataTables */
function dataTables(){
	// 체크박스 있는 버전
	$('.checkDataTable').DataTable({
		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: {
				pageLength: {
					menu: [ 10, 25, 50, 100 ]
				}
			},
			bottomEnd: {
				info : true,
				paging: {
					buttons: 5
				},
			},
		},
		searching: false,
		ordering: false,
		language :{
			emptyTable: "데이터가 없습니다.",
		},
		columnDefs: [
			{
				render: DataTable.render.select(),
				targets: 0
			}
		],
		select: {
			style: 'multi',
			selector: 'td:first-child'
		},
		destroy: true
	});
	
	// 체크박스 단일선택 버전
	$('.onlyCheckDataTable').DataTable({
		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: {
				pageLength: {
					menu: [ 10, 25, 50, 100 ]
				}
			},
			bottomEnd: {
				info : true,
				paging: {
					buttons: 5
				},
			},
		},
		searching: false,
		ordering: false,
		language :{
			emptyTable: "데이터가 없습니다.",
		},
		columnDefs: [
			{
				render: DataTable.render.select(),
				targets: 0
			}
		],
		select: {
			style: 'single',
			selector: 'td:first-child'
		},
		destroy: true
	});

	// 체크박스 없는 버전
	$('.normalDataTable').DataTable({

		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: {
				pageLength: {
					menu: [ 10, 25, 50, 100 ]
				}
			},
			bottomEnd: {
				info : true,
				paging: {
					buttons: 5
				},
			},
		},
		searching: false,
		ordering: false,
		language :{
			emptyTable: "데이터가 없습니다.",
		},
		destroy: true
	});
	
	// 옵션 없는 버전
	$('.noOptionDataTable').DataTable({

		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: null,
			bottomEnd: null,
		},
		searching: false,
		ordering: false,
		language :{
			emptyTable: "데이터가 없습니다.",
		},
		columnDefs: [
			{
				render: DataTable.render.select(),
				targets: 0
			}
		],
		select: {
			style: 'multi',
			selector: 'td:first-child'
		},
		destroy: true
	});
	
	// 옵션과 체크 없는 버전
	$('.noOptionNoCheckDataTable').DataTable({

		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: null,
			bottomEnd: null,
		},
		searching: false,
		ordering: false,
		language :{
			emptyTable: "데이터가 없습니다.",
		},
		destroy: true
	});
}

/* 설정 > 부서 및 사용자 설정 */
// IP 체크박스 여부
function changeModifyAreaBox(){
	if($(".userModifyPopup input[name='ipPlus']").prop("checked") == true){
		$(".userModifyPopup .ipTable").removeClass("hide");
		$(".userModifyPopup .addIp .bu_wrap button").removeClass("disable");
	}else{
		$(".userModifyPopup .ipTable").addClass("hide");
		$(".userModifyPopup .addIp .bu_wrap button").addClass("disable");
	}
}
function changeAddAreaBox(){
	if($(".userAddPopup input[name='ipPlus']").prop("checked") == true){
		$(".userAddPopup .ipTable").removeClass("hide");
		$(".userAddPopup .addIp .bu_wrap button").removeClass("disable");
	}else{
		$(".userAddPopup .ipTable").addClass("hide");
		$(".userAddPopup .addIp .bu_wrap button").addClass("disable");
	}
}

/**
 * 데이터 테이블 공통
 */
function initDataTables(tableId, opts) {
	// 기본 옵션
	var commOpts = {
		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: {
				pageLength: {
					menu: [ 10, 25, 50, 100 ]
				}
			},
			bottomEnd: {
				info : true,
				paging: {
					buttons: 5
				},
			},
		},
		searching: true,
		ordering: false,
		language :{
			emptyTable: "No Data.",
		},
		destroy: true
	};
	// 데이터 테이블 적용 옵션
	var dataTableOpts = {};
	if (opts) {
		dataTableOpts = Object.assign({}, commOpts, opts);
	} else {
		dataTableOpts = commOpts;
	}

	return $(tableId).DataTable(dataTableOpts);
}

/* datepicker 공통 */
/*function datePicker(){
		
	// 오늘/이번주/이번달
	$(".datePickerInner").datepicker({
		dateFormat: "yy-mm-dd", // yyyy-mm-dd 형식
		showAnim: "slideDown", // 애니메이션 효과
		onSelect: function () {
			// 날짜 수동 변경 시 "직접입력"으로 옵션 변경
			$("#dateSelect").val("custom");
		}
	});
	
	// Selectbox 변경 시 이벤트
	$("#dateSelect").on("change", function () {
		var selectedOption = $(this).val();
		var today = new Date();
		var startDate = "";
		var endDate = "";
	
		if (selectedOption === "today") {
			// 오늘 날짜
			startDate = endDate = $.datepicker.formatDate("yy-mm-dd", today);
		} else if (selectedOption === "this-week") {
			// 이번 주: 월요일 ~ 일요일 기준
			var dayOfWeek = today.getDay(); // 요일 (0: 일요일, 1: 월요일, ..., 6: 토요일)
			var monday = new Date(today); // 이번 주 월요일
			monday.setDate(today.getDate() - (dayOfWeek === 0 ? 6 : dayOfWeek - 1)); // 일요일인 경우 6일 전으로, 나머지는 적절히 계산
	
			var sunday = new Date(monday); // 이번 주 일요일
			sunday.setDate(monday.getDate() + 6);
	
			startDate = $.datepicker.formatDate("yy-mm-dd", monday);
			endDate = $.datepicker.formatDate("yy-mm-dd", sunday);
		} else if (selectedOption === "this-month") {
			// 이번 달 시작일과 종료일
			var startOfMonth = new Date(today.getFullYear(), today.getMonth(), 1);
			var endOfMonth = new Date(today.getFullYear(), today.getMonth() + 1, 0); // 이번 달 마지막 날
	
			startDate = $.datepicker.formatDate("yy-mm-dd", startOfMonth);
			endDate = $.datepicker.formatDate("yy-mm-dd", endOfMonth);
		} else if (selectedOption === "custom") {
			// "직접입력": 필드 비워둠
			$(".datePickerInner.start-date, .datePickerInner.end-date").val("");
			return;
		} else {
			// "전체" 선택 시 필드 초기화
			$(".datePickerInner.start-date, .datePickerInner.end-date").val("");
			return;
		}
	
		// Datepicker 필드에 값 설정 (비활성화 제거)
		$(".datePickerInner.start-date").val(startDate);
		$(".datePickerInner.end-date").val(endDate);
	});
	
	// Datepicker 수동 입력 감지
	$(".datePickerInner").on("change", function () {
		var selectedOption = $("#dateSelect").val();
		if (selectedOption === "this-week" || selectedOption === "this-month") {
			// 현재 선택된 범위의 날짜
			var selectedStartDate = $(".datePickerInner.start-date").val();
			var selectedEndDate = $(".datePickerInner.end-date").val();
	
			// 이번 주/이번 달의 기본 날짜 계산
			var today = new Date();
			var startOfPeriod, endOfPeriod;
	
			if (selectedOption === "this-week") {
				// 이번 주: 월요일 ~ 일요일 기준
				var dayOfWeek = today.getDay();
				startOfPeriod = new Date(today);
				startOfPeriod.setDate(today.getDate() - (dayOfWeek === 0 ? 6 : dayOfWeek - 1)); // 월요일 계산
				endOfPeriod = new Date(startOfPeriod);
				endOfPeriod.setDate(startOfPeriod.getDate() + 6); // 일요일 계산
			} else if (selectedOption === "this-month") {
				// 이번 달 시작일과 종료일
				startOfPeriod = new Date(today.getFullYear(), today.getMonth(), 1);
				endOfPeriod = new Date(today.getFullYear(), today.getMonth() + 1, 0);
			}
	
			// yyyy-mm-dd 형식으로 변환
			var formattedStart = $.datepicker.formatDate("yy-mm-dd", startOfPeriod);
			var formattedEnd = $.datepicker.formatDate("yy-mm-dd", endOfPeriod);
	
			// 기본 날짜와 다르면 "직접입력"으로 변경
			if (selectedStartDate !== formattedStart || selectedEndDate !== formattedEnd) {
				$("#dateSelect").val("custom");
			}
		}
	});
}*/
/* datepicker 공통 */
function datePicker() {
    function setDatePicker(selector, dateSelectSelector) {
        $(selector).datepicker({
            dateFormat: "yy-mm-dd",
            showAnim: "slideDown",
            onSelect: function () {
                $(dateSelectSelector).val("custom");
            }
        });

        $(dateSelectSelector).on("change", function () {
            var selectedOption = $(this).val();
            var today = new Date();
            var startDate = "";
            var endDate = "";

            if (selectedOption === "today") {
                startDate = endDate = $.datepicker.formatDate("yy-mm-dd", today);
            } else if (selectedOption === "this-week") {
                var dayOfWeek = today.getDay();
                var monday = new Date(today);
                monday.setDate(today.getDate() - (dayOfWeek === 0 ? 6 : dayOfWeek - 1));

                var sunday = new Date(monday);
                sunday.setDate(monday.getDate() + 6);

                startDate = $.datepicker.formatDate("yy-mm-dd", monday);
                endDate = $.datepicker.formatDate("yy-mm-dd", sunday);
            } else if (selectedOption === "this-month") {
                var startOfMonth = new Date(today.getFullYear(), today.getMonth(), 1);
                var endOfMonth = new Date(today.getFullYear(), today.getMonth() + 1, 0);

                startDate = $.datepicker.formatDate("yy-mm-dd", startOfMonth);
                endDate = $.datepicker.formatDate("yy-mm-dd", endOfMonth);
            } else if (selectedOption === "custom") {
                $(selector).val("");
                return;
            } else {
                $(selector).val("");
                return;
            }

            $(selector + ".start-date").val(startDate);
            $(selector + ".end-date").val(endDate);
        });

        $(selector).on("change", function () {
            var selectedOption = $(dateSelectSelector).val();
            if (selectedOption === "this-week" || selectedOption === "this-month") {
                var selectedStartDate = $(selector + ".start-date").val();
                var selectedEndDate = $(selector + ".end-date").val();

                var today = new Date();
                var startOfPeriod, endOfPeriod;

                if (selectedOption === "this-week") {
                    var dayOfWeek = today.getDay();
                    startOfPeriod = new Date(today);
                    startOfPeriod.setDate(today.getDate() - (dayOfWeek === 0 ? 6 : dayOfWeek - 1));
                    endOfPeriod = new Date(startOfPeriod);
                    endOfPeriod.setDate(startOfPeriod.getDate() + 6);
                } else if (selectedOption === "this-month") {
                    startOfPeriod = new Date(today.getFullYear(), today.getMonth(), 1);
                    endOfPeriod = new Date(today.getFullYear(), today.getMonth() + 1, 0);
                }

                var formattedStart = $.datepicker.formatDate("yy-mm-dd", startOfPeriod);
                var formattedEnd = $.datepicker.formatDate("yy-mm-dd", endOfPeriod);

                if (selectedStartDate !== formattedStart || selectedEndDate !== formattedEnd) {
                    $(dateSelectSelector).val("custom");
                }
            }
        });
    }

    // 첫 번째 datepicker 설정
    setDatePicker(".datePickerInner", "#dateSelect");

    // 두 번째 datepicker (제품 정책 변경 등 팝업)
    setDatePicker(".datePickerPopProduct", "#dateSelectPopProduct");

    // 세 번째 datepicker (이동 파일 이력 검색 팝업)
    setDatePicker(".datePickerPopMfile", "#dateSelectPopMfile");
}


/* 프로필 팝업 */
async function openProfilePopup() {
	$.ajax({
		url: '/primx/fun/usr/myInfo.do',

		type: 'GET',
		success: function(response) {
			if (response && response.data) {
				$("#profilePopupUserId").val(response.data.emplyrId);
				$("#profilePopupUserName").val(response.data.userName);
				$("#profilePopupUserEmail").val(response.data.email);

				$("#profilePopupUserExpireDate").val(response.data.expiredDate);
				$("#profilePopupUserPasswordExpireDate").val(response.data.pwdExpiredDate);
				
				// 계정연장신청 버튼 활성화 유무 함수
				checkAccountExtension(response.data.userRole);
			}
		},
		error: function(xhr, status, error) {
			console.error("사용자 정보 조회 실패:", {
				status: status,
				error: error,
				response: xhr.responseText
			});
		}
	})
	const roleData = await loadRoleInfo(0);
	$("#profilePopupUserRole").val(roleData.data[0].authorDescription);

	$(".profilePopup").removeClass("hide");
}

async function checkAccountExtension(userRole) {
	$("#accountExtensionBtn").addClass("hide");
	var notice = await loadNotice();
	for(var i = 0; i < notice.data.length; i++) {
		if(notice.data[i].useYn == 'Y' && notice.data[i].requestApprovalCode == 'C') {
			if(userRole.includes('ADMIN') && notice.data[i].requestUserRole == 'ADMIN') {
				$("#accountExtensionBtn").removeClass("hide");
			}
			else if(userRole.includes('USER') && notice.data[i].requestUserRole == 'USER') {
				$("#accountExtensionBtn").removeClass("hide");
			}
		}
	}
}

function saveProfile() {
	
	const emplyrId = $('#profilePopupUserId').val().trim();
	const userName = $('#profilePopupUserName').val().trim();
	const email = $('#profilePopupUserEmail').val().trim();
	
	$(".saveBtn").addClass("hide");
	$(".modifyProfileBtn").removeClass("hide");
	$("input.modifyYes").addClass("nowrite");

	// 사용자 등록 데이터 준비
	const userRegistrationData = {
		emplyrId: emplyrId,
		userName: userName,
		email: email,
		usePeriod: 2
	};

	// 사용자 등록 요청
	$.ajax({
		url: '/primx/fun/usr/updateUser.do', // 사용자 정보 업데이트 요청 URL
		type: 'POST',
		contentType: 'application/json',
		data: JSON.stringify(userRegistrationData),
		success: function(response) {
			if (response.success) {
				alert('프로필 수정이 완료되었습니다.');
				$('.userModifyPopup').addClass('hide');
			} else {
				alert(response.message);
			}
		},
		error: function(xhr, status, error) {
			console.error("사용자 수정 실패:", {
				status: status,
				error: error,
				response: xhr.responseText
			});
			alert("사용자 수정 중 오류가 발생했습니다.");
		}
	});
}

function initChangePasswordPopup() {

	$("#changePasswordPopupCurrentPassword").val("");
	$("#changePasswordPopupNewPassword").val("");
	$("#changePasswordPopupNewPasswordConfirm").val("");
	$('.changePasswordPopup').removeClass('hide');
}

/* 비밀번호 변경 */
async function updatePassword() {
	var emplyrId = $("#profilePopupUserId").val();
	var currentPassword = $("#changePasswordPopupCurrentPassword").val();
	var newPassword = $("#changePasswordPopupNewPassword").val();
	var newPasswordConfirm = $("#changePasswordPopupNewPasswordConfirm").val();

	if (currentPassword === "") {
		alert("현재 비밀번호를 입력해주세요.");
		return;
	}
	if (newPassword === "") {
		alert("신규 비밀번호를 입력해주세요.");
		return;
	}
	if (newPasswordConfirm === "") {
		alert("비밀번호 확인을 입력해주세요.");
		return;
	}
	if (newPassword !== newPasswordConfirm) {
		alert("비밀번호가 일치하지 않습니다.");
		return;
	}

	// 비밀번호 검사
	const passwordCheck = await checkCommonPassword(newPassword);
	console.log("passwordCheck: "+passwordCheck);
	if (passwordCheck !== 0) {
		return;
	}

	$.ajax({
		url: '/primx/fun/usr/updatePassword.do',
		type: 'POST',
		data: {
			emplyrId: emplyrId,	
			currentPassword: currentPassword,
			newPassword: newPassword
		},
		success: function(response) {
			if (response.success) {
				alert("비밀번호 변경 완료: " + response.message);
				$("#changePasswordPopupCurrentPassword").val("");
				$("#changePasswordPopupNewPassword").val("");
				$("#changePasswordPopupNewPasswordConfirm").val("");
				$('.changePasswordPopup').addClass('hide');
				openProfilePopup();
			} else {
				alert("비밀번호 변경 실패: " + response.message);
			}
		},
		error: function(xhr, status, error) {
			alert("비밀번호 변경 실패: " + error);
		}
	});
}

async function openAccountExtensionPopup() {
	callPageClass = '.createRequestPopup.innerVerPop';
    // 신청일자 추가 년월일 시분초
    $('#requestDate').text(moment().format('YYYY-MM-DD HH:mm:ss'));

    try {
        // 사용자 정보 로드 - await를 사용하여 비동기 처리
		const myInfo = await loadMyInfo();
		console.log('myInfo:', myInfo);

		// 신청 팝업 초기화
		clearCreateRequestPopup(myInfo);

		// 롤권한 정보 로드
		setNotice(myInfo.userRole, '.createRequestPopup #createRequestPopupNotice');
		setReason(myInfo.userRole, '.createRequestPopup #requestReason');
		// setFormUseYn(myInfo.userRole, '.createRequestPopup');

		// 기본값 서식을 계정연장 신청으로 지정
		$('#requestApprovalCode').val('C');
		// 서식 선택 비활성화
		$('#requestApprovalCode').prop('disabled', true);
		$('#moveFielViewBtn').addClass('hide');
		
		// 모든 서식 숨기기
		$('.approvalFormTypeM').addClass('hide');
		$('.approvalFormTypeC').removeClass('hide');
		$('.approvalFormTypeA').addClass('hide');

		// 사용자 이름 설정
		$('#requestUserNm').text(myInfo.userName);
		$('#requestUserId').val(myInfo.emplyrId);

        // 서식 : 사용자 등록 신청 시
        // 회사 정보 로드
        // loadCompanyInfo($('.createRequestPopup #companyId'));

		
		// console.log("loadCompanyInfo 호출 후");
        // 롤권한 정보 로드
        // loadRoleInfo(1);


		// 신청 팝업 날짜 피커 이벤트 설정
		createRequestPopupDatePickerEvent();

    } catch (error) {
        console.error('사용자 정보 로드 실패2:', error);
        alert('사용자 정보를 불러오는데 실패했습니다2.');
    }

	$(".createRequestPopup.innerVerPop").removeClass("hide");
}

function openLicensePopup() {
	$('#spanFile1').text('라이선스 암호화 파일');
	$('#spanFile2').text('암호화 해제 키 파일');
	// upload1의 파일이 선택되면 라이선스 암호화 파일명을 spanFile에 표시
	$('#upload1').on('change', function() {
		const fileName = $(this).val().split('\\').pop(); // 파일명 추출
		$('#spanFile1').text(fileName); // span에 파일명 표시
	});
	
	// upload2의 파일이 선택되면 라이선스 암호화 해제 키 파일명을 spanFile에 표시
	$('#upload2').on('change', function() {
		const fileName = $(this).val().split('\\').pop(); // 파일명 추출
		$('#spanFile2').text(fileName); // span에 파일명 표시
	});

	$(".licenseRegiPopup").removeClass("hide");
}


/* 라이선스 기능 */
function saveLicense() {
	// FormData로 라이선스 파일 정보를 가져온다
	var formData = new FormData();
	var encryptedFile = $("#upload1")[0].files[0]; // 라이선스 암호화 파일
	var keyFile = $("#upload2")[0].files[0]; // 암호화 해제 키 파일

	if (encryptedFile) {
		formData.append("encryptedFile", encryptedFile);
	}
	if (keyFile) {
		formData.append("keyFile", keyFile);
	}
	console.log("encryptedFile: "+encryptedFile);
	console.log("keyFile: "+keyFile);

	$.ajax({
		url: '/primx/fun/sys/updateLicense.do',
		type: 'POST',
		data: formData,
		processData: false,
		contentType: false,
		success: function(response) {
			if (response.status === "success") {
				alert("라이선스 등록이 완료되었습니다.");
				closeLicensePopup(); // 팝업 닫기
			} else {
				alert("라이선스 등록에 실패했습니다.");
			}
		},
		error: function(xhr, status, error) {
			alert("오류 발생: " + error);
		}
	});
	
}

function closeLicensePopup() {
	$(".licenseRegiPopup").addClass("hide");
}

async function openSettingPopup() {
    try {
        // 회사 정보 불러오기
        const companyData = await loadRootCompanyInfo();
        $('#companyName').val(companyData.deptName);
        $('#companyCode').val(companyData.companyId);

        // 라이선스 정보 불러오기
        loadLicenseInfo();

        // 비밀번호 설정 정보 불러오기
        loadPasswordSettingInfo();

		// 운영 정보 불러오기
		loadOperationInfo();

		// 탭 초기화
		$(".topSetPopup .tabCont > li").removeClass("on"); //다음액션 전의 것들은 초기화
		$(".topSetPopup .tabTitle li").removeClass("on"); //모든 li의 테두리가 지워져라
		$('.topSetPopup .tabTitle li:first-child').addClass("on"); //첫번째 li에 테두리를 추가해라
		$('.topSetPopup .tabCont > li:first-child').addClass("on"); //첫번째 탭을 선택해라


        $(".settingPopup").removeClass("hide");
    } catch (error) {
        alert("회사 정보를 불러오는데 실패했습니다: " + error);
    }
}

function loadRootCompanyInfo() {
    return new Promise((resolve, reject) => {
        $.ajax({
            url: '/primx/fun/dpt/loadRootCompanyInfo.do',
            type: 'GET',
            success: function(response) {
                if (response.success) {
                    resolve(response.data);
                } else {
                    reject(response.message);
                }
            },
            error: function(xhr, status, error) {
                reject(error);
            }
        });
    });
}


function loadLicenseInfo() {

	// 라이선스 정보 불러오기
	dataTable = initDataTables('#licenseDetailTable', {
		ajax: {
			url: '/primx/fun/sys/loadLicenseInfo.do',
			type: 'GET'
		},
		layout: {
			topStart: null,
			topEnd: null,
			bottomStart: null,
			bottomEnd: null
		},
		columns: [
			// 번호, 상태, 제품명, 수량, 시작일, 종료일, 일반ID, 제품키
			{ data: null, render: function(data, type, row, meta) {
				return meta.row + meta.settings._iDisplayStart + 1;
			} },
			// USING: 사용 또는 만료
			{ data: 'statusCd', render: function(data) {
				return data === 'USING' ? '사용' : '만료';
			} },
			{ data: 'productName' },
			{ data: 'quantity' },
			{ data: 'startDate' },
			{ data: 'endDate' }
		]
	});
}

// 비밀번호 인증 
function loadPasswordSettingInfo() {
	$.ajax({
		url: '/primx/fun/cmm/loadPasswordSettingInfo.do',
		type: 'GET',
		success: function(response) {
			console.log("loadPasswordSettingInfo: "+response.data);
			if(response.data != null) {
				$("#passMinLength").val(response.data.passMinLength);
				$("#passCombUppercase").val(response.data.passCombUppercase);
				$("#passCombDigit").val(response.data.passCombDigit);
				$("#passCombSymbol").val(response.data.passCombSymbol);
				$("#passExpireDays").val(response.data.passExpireDays);
				$("#passNotifyBefore").val(response.data.passNotifyBefore);
				
				// 체크박스 상태 설정 및 입력 필드 disabled 상태 설정
				if (response.data.passMinLength != 0) {
					$("#minimumLengthCheckbox").prop("checked", true);
					$("#passMinLength").prop("disabled", false);
				} else {
					$("#minimumLengthCheckbox").prop("checked", false);
					$("#passMinLength").prop("disabled", true);
				}
				
				if (response.data.passCombUppercase != 0) {
					$("#uppercaseCheckbox").prop("checked", true);
					$("#passCombUppercase").prop("disabled", false);
				} else {
					$("#uppercaseCheckbox").prop("checked", false);
					$("#passCombUppercase").prop("disabled", true);
				}
				
				if (response.data.passCombDigit != 0) {
					$("#numbersCheckbox").prop("checked", true);
					$("#passCombDigit").prop("disabled", false);
				} else {
					$("#numbersCheckbox").prop("checked", false);
					$("#passCombDigit").prop("disabled", true);
				}
				
				if (response.data.passCombSymbol != 0) {
					$("#specialCharactersCheckbox").prop("checked", true);
					$("#passCombSymbol").prop("disabled", false);
				} else {
					$("#specialCharactersCheckbox").prop("checked", false);
					$("#passCombSymbol").prop("disabled", true);
				}
				
				if (response.data.passNotifyBefore != 0) {
					$("#expirationNotificationCheckbox").prop("checked", true);
					$("#passNotifyBefore").prop("disabled", false);
				} else {
					$("#expirationNotificationCheckbox").prop("checked", false);
					$("#passNotifyBefore").prop("disabled", true);
				}
				
				// 사용기간 0이 아니면 usagePeriodType 1 설정
				if (response.data.passExpireDays == 0) {
					$("#usagePeriodType").val(0);
					$("#passExpireDays").prop("disabled", true);
				} else {
					$("#usagePeriodType").val(1);
					$("#passExpireDays").prop("disabled", false);
				}
			} else {
				alert("비밀번호 인증 설정 정보가 등록되어있지 않습니다.");
			}
		},
		error: function(xhr, status, error) {
			alert("오류 발생: " + error);
		}
	});
}

async function loadOperationInfo() {
	// 인사연동 로드
	await loadHumanResourceInfo();
	
	// 미접속 시스템 삭제 로드
	await loadUnusedSystemDeleteInfo();

	// 데이터 정리 로드
	await loadDataCleanupInfo();

	// agent 접속 로드
	await loadAgentAccessInfo();

}

function loadServerConfig(keyName) {
	return new Promise((resolve, reject) => {
		$.ajax({
			url: '/primx/fun/sys/loadServerConfig.do',
			type: 'POST',
			data: {
				keyName: keyName
			},
			success: function(response) {
				// console.log("loadServerConfig: "+ JSON.stringify(response.data));
				resolve(response.data);
			},
		error: function(xhr, status, error) {
				reject(error);
			}
		});
	});
}

async function loadHumanResourceInfo() {
    // HrSync 설정 데이터 로드
    var hrSync = await loadServerConfig('HrSync');
    // 리턴 결과가 없거나 에러인 경우 처리
    if (hrSync == null || hrSync.valueData == null) {
        return;
    }
    
    // JSON 문자열을 객체로 파싱
    const hrSyncData = JSON.parse(hrSync.valueData);
    
    // useYn 값에 따라 체크박스 상태 설정
    $("#hrIntegrationCheckbox").prop("checked", hrSyncData.useYn === 'Y');
	
    // 체크박스 상태에 따라 select 요소들의 disabled 속성 설정
    const isChecked = hrSyncData.useYn === 'Y';
    $('#operationCycle, #weeklyDay, #monthlyDay, #integrationHour, #integrationMinute').prop('disabled', !isChecked);
	if(isChecked) {
		$("#integrationTestBtn").prop("disabled", !isChecked);
		$("#integrationTestBtn").removeClass("disable");
		$("#integrationExecuteBtn").prop("disabled", !isChecked);
		$("#integrationExecuteBtn").removeClass("disable");
	} 
	else {
		$("#integrationTestBtn").prop("disabled", !isChecked);
		$("#integrationTestBtn").addClass("disable");
		$("#integrationExecuteBtn").prop("disabled", !isChecked);
		$("#integrationExecuteBtn").addClass("disable");

	}

    // time 값을 시간과 분으로 분리 (예: "00:00" -> ["00", "00"])
    const [hours, minutes] = hrSyncData.time.split(':');
    
    // 각 selectbox에 값 설정
    $('#operationCycle').val(hrSyncData.cycle);
    $('#weeklyDay').val(hrSyncData.day);
    $('#monthlyDay').val(hrSyncData.date);
    $('#integrationHour').val(hours);
    $('#integrationMinute').val(minutes);

    // 동작주기 선택에 따른 박스 표시/숨김 처리
    const selectedCycle = hrSyncData.cycle;
    if (selectedCycle === 'daily') {
        $('#weeklyBox').addClass('hide');
        $('#monthlyBox').addClass('hide');
    } else if (selectedCycle === 'weekly') {
        $('#weeklyBox').removeClass('hide');
        $('#monthlyBox').addClass('hide');
    } else if (selectedCycle === 'monthly') {
        $('#weeklyBox').addClass('hide');
        $('#monthlyBox').removeClass('hide');
    }
}

// 동작주기 변경 이벤트 핸들러 추가
$(document).ready(function() {
    // 인사연동 체크박스 이벤트 핸들러
    $('#hrIntegrationCheckbox').on('change', function() {
        const isChecked = $(this).is(':checked');
        $('#operationCycle, #weeklyDay, #monthlyDay, #integrationHour, #integrationMinute').prop('disabled', !isChecked);
    });

    // 미접속 시스템 삭제 체크박스 이벤트 핸들러
    $('#disconnectSystemDeleteCheckbox').on('change', function() {
        const isChecked = $(this).is(':checked');
        $('#term, #deleteHour, #deleteMinute').prop('disabled', !isChecked);
    });

    $('#operationCycle').on('change', function() {
        const selectedValue = $(this).val();
        if (selectedValue === 'daily') {
            $('#weeklyBox').addClass('hide');
            $('#monthlyBox').addClass('hide');
        } else if (selectedValue === 'weekly') {
            $('#weeklyBox').removeClass('hide');
            $('#monthlyBox').addClass('hide');
        } else if (selectedValue === 'monthly') {
            $('#weeklyBox').addClass('hide');
            $('#monthlyBox').removeClass('hide');
        }
    });
});

async function loadUnusedSystemDeleteInfo() {
    var inactiveDel = await loadServerConfig('InactiveDel');
    // 리턴 결과가 없거나 에러인 경우 처리
    if (inactiveDel == null || inactiveDel.valueData == null) {
        return;
    }
    
    // JSON 문자열을 객체로 파싱
    const inactiveDelData = JSON.parse(inactiveDel.valueData);
    
    // useYn 값에 따라 체크박스 상태 설정
    $("#disconnectSystemDeleteCheckbox").prop("checked", inactiveDelData.useYn === 'Y');
    
    // 체크박스 상태에 따라 select와 input 텍스트박스의 disabled 속성 설정
    const isChecked = inactiveDelData.useYn === 'Y';
	console.log("isChecked: "+isChecked);
    $('#term, #deleteHour, #deleteMinute').prop('disabled', !isChecked);
	if(isChecked) {
		$("#viewUnconnectedSystemBtn").prop("disabled", !isChecked);
		$("#viewUnconnectedSystemBtn").removeClass("disable");
	}
	else {
		$("#viewUnconnectedSystemBtn").prop("disabled", !isChecked);
		$("#viewUnconnectedSystemBtn").addClass("disable");
	}
    
    // time 값을 시간과 분으로 분리 (예: "00:00" -> ["00", "00"])
    const [hours, minutes] = inactiveDelData.time.split(':');
    
    // #term 값 설정
    $("#term").val(inactiveDelData.term);
    // #deleteHour 값 설정
    $("#deleteHour").val(hours);
    // #deleteMinute 값 설정
    $("#deleteMinute").val(minutes);
}

async function loadDataCleanupInfo() {
    var dataCleanOp = await loadServerConfig('DataCleanOp');
    // dataCleanOp 데이터가 있는경우 처리
    if (dataCleanOp != null && dataCleanOp.valueData != null) {
        // JSON 문자열을 객체로 파싱
        const dataCleanOpData = JSON.parse(dataCleanOp.valueData);
        
        // useYn 값에 따라 체크박스 상태 설정
        $("#operationHistoryDataDeleteCheckbox").prop("checked", dataCleanOpData.op_hist.useYn === 'Y');
        
        // 체크박스 상태에 따라 input과 select 요소들의 disabled 속성 설정
        const isChecked = dataCleanOpData.op_hist.useYn === 'Y';
        $('#periodSelect, #startDatePicker, #endDatePicker, #operationHour, #operationMinute').prop('disabled', !isChecked);
        
        // #cond 값 설정
        $("#periodSelect").val(dataCleanOpData.op_hist.cond);
        
        // 기간지정이 아닌 경우 datepicker 비활성화
        if (dataCleanOpData.op_hist.cond !== '') {
            $('#startDatePicker, #endDatePicker').prop('disabled', true);
        }
        
        // #start 값 설정
        $("#startDatePicker").val(dataCleanOpData.op_hist.start);
        // #end 값 설정
        $("#endDatePicker").val(dataCleanOpData.op_hist.end);
        
        // time 값을 시간과 분으로 분리 (예: "00:00" -> ["00", "00"])
        const [hours, minutes] = dataCleanOpData.op_hist.time.split(':');
        // #time 값 설정
        $("#operationHour").val(hours);
        $("#operationMinute").val(minutes);
    }

    var dataCleanPd = await loadServerConfig('DataCleanPd');
    // dataCleanPd 데이터가 있는경우 처리
    if (dataCleanPd != null && dataCleanPd.valueData != null) {
        // JSON 문자열을 객체로 파싱
        const dataCleanPdData = JSON.parse(dataCleanPd.valueData);
        
        // useYn 값에 따라 체크박스 상태 설정
        $("#productHistoryDataDeleteCheckbox").prop("checked", dataCleanPdData.prod_hist.useYn === 'Y');
        
        // 체크박스 상태에 따라 input과 select 요소들의 disabled 속성 설정
        const isChecked = dataCleanPdData.prod_hist.useYn === 'Y';
        $('#periodSelect2, #startDatePicker2, #endDatePicker2, #productHour, #productMinute').prop('disabled', !isChecked);
        
        // #cond 값 설정
        $("#periodSelect2").val(dataCleanPdData.prod_hist.cond);
        
        // 기간지정이 아닌 경우 datepicker 비활성화
        if (dataCleanPdData.prod_hist.cond !== '') {
            $('#startDatePicker2, #endDatePicker2').prop('disabled', true);
        }
        
        // #start 값 설정
        $("#startDatePicker2").val(dataCleanPdData.prod_hist.start);
        // #end 값 설정
        $("#endDatePicker2").val(dataCleanPdData.prod_hist.end);
        
        // time 값을 시간과 분으로 분리 (예: "00:00" -> ["00", "00"])
        const [hours, minutes] = dataCleanPdData.prod_hist.time.split(':');
        // #time 값 설정
        $("#productHour").val(hours);
        $("#productMinute").val(minutes);
    }
}

async function loadAgentAccessInfo() {
    // 데이터 샘플) 24	AgentConn	{"keepalive":"3","log_push":"1","state_push":"1"}

    // var agentConn = await loadServerConfig('AgentConn');
    var agentKeepAliveMinuteInterval = await loadServerConfig('AgentKeepAliveMinuteInterval');
    var agentProductLogSendMinuteInterval = await loadServerConfig('AgentProductLogSendMinuteInterval');

    // 각 input에 값 설정
    $("#keepalive").val(agentKeepAliveMinuteInterval.valueData);
    $("#logPush").val(agentProductLogSendMinuteInterval.valueData);
}


function saveServerConfig() {
	console.log("saveServerConfig");

	var serverConfigVOList = [
		{
			keyName: 'HrSync',
			valueData: JSON.stringify({
				useYn: $('#hrIntegrationCheckbox').is(":checked") ? 'Y' : 'N',
				cycle: $('#operationCycle').val(),
				day: $('#weeklyDay').val(),
				date: $('#monthlyDay').val(),
				time: $('#integrationHour').val() + ':' + $('#integrationMinute').val()
			})
		},
		{
			keyName: 'InactiveDel',
			valueData: JSON.stringify({
				useYn: $('#disconnectSystemDeleteCheckbox').is(":checked") ? 'Y' : 'N',
				term: $('#term').val(),
				time: $('#deleteHour').val() + ':' + $('#deleteMinute').val()
			})
		},	
		{
			keyName: 'DataCleanOp',
			valueData: JSON.stringify({
				op_hist: {
					useYn: $('#operationHistoryDataDeleteCheckbox').is(":checked") ? 'Y' : 'N',
					cond: $('#periodSelect').val(),
					start: $('#startDatePicker').val(),
					end: $('#endDatePicker').val(),
					time: $('#operationHour').val() + ':' + $('#operationMinute').val()
				}
			})
		},
		{
			keyName: 'DataCleanPd',
			valueData: JSON.stringify({
				prod_hist: {
					useYn: $('#productHistoryDataDeleteCheckbox').is(":checked") ? 'Y' : 'N',
					cond: $('#periodSelect2').val(),
					start: $('#startDatePicker2').val(),
					end: $('#endDatePicker2').val(),
					time: $('#productHour').val() + ':' + $('#productMinute').val()
				}
			})
		},
		{
			keyName: 'AgentKeepAliveMinuteInterval',
			valueData: $('#keepalive').val()
		},
		{
			keyName: 'AgentProductLogSendMinuteInterval',
			valueData: $('#logPush').val()
		}
		// {
		// 	keyName: 'AgentConn',
		// 	valueData: JSON.stringify({
		// 		keepalive: $('#keepalive').val(),
		// 		log_push: $('#logPush').val(),
		// 		state_push: $('#statePush').val()
		// 	})
		// }
	];

	console.log("serverConfigVOList: "+JSON.stringify(serverConfigVOList));

	// 서버 설정 정보 저장
	$.ajax({
		url: '/primx/fun/sys/saveServerConfig.do',
		type: 'POST',
		data: JSON.stringify(serverConfigVOList),
		contentType: 'application/json',
		success: function(response) {
			if (response.status === "success") {
				alert("서버 설정 정보가 저장되었습니다.");
				closeSettingPopup();
			} else {
				alert("서버 설정 정보 저장에 실패했습니다.");
			}
		},
		error: function(xhr, status, error) {
			alert("오류 발생: " + error);
		}
	});
	
}


function savePasswordSetting() {
	console.log("savePasswordSetting");

	// 사용자 등급 확인
	const userRole = "USER";

    // 최소길이 체크박스 체크 여부 확인
    const minimumLengthCheckbox = $("#minimumLengthCheckbox").is(":checked");
    let passMinLength = $("#passMinLength").val(); // let으로 변경
    // 대문자 체크박스 체크 여부 확인
    const uppercaseCheckbox = $("#uppercaseCheckbox").is(":checked");
    let passCombUppercase = $("#passCombUppercase").val(); // let으로 변경
    // 숫자 체크박스 체크 여부 확인
    const numbersCheckbox = $("#numbersCheckbox").is(":checked");
    let passCombDigit = $("#passCombDigit").val(); // let으로 변경
    // 특수문자 체크박스 체크 여부 확인
    const specialCharactersCheckbox = $("#specialCharactersCheckbox").is(":checked");
    let passCombSymbol = $("#passCombSymbol").val(); // let으로 변경

    // 사용기간 타입 체크박스 체크 여부 확인
    const usagePeriodType = $("#usagePeriodType").val();
    let passExpireDays = $("#passExpireDays").val(); // let으로 변경
    // 만료알림 체크박스 체크 여부 확인
    const expirationNotificationCheckbox = $("#expirationNotificationCheckbox").is(":checked");
    let passNotifyBefore = $("#passNotifyBefore").val(); // let으로 변경

	console.log("minimumLengthCheckbox: "+minimumLengthCheckbox);
	console.log("passMinLength: "+passMinLength);
	console.log("uppercaseCheckbox: "+uppercaseCheckbox);
	console.log("passCombUppercase: "+passCombUppercase);
	console.log("numbersCheckbox: "+numbersCheckbox);
	console.log("passCombDigit: "+passCombDigit);
	console.log("specialCharactersCheckbox: "+specialCharactersCheckbox);
	console.log("usagePeriodType: "+usagePeriodType);
	console.log("passExpireDays: "+passExpireDays);
	console.log("expirationNotificationCheckbox: "+expirationNotificationCheckbox);
	console.log("passNotifyBefore: "+passNotifyBefore);

	// 체크박스가 설정되어있지 않는 경우 기본값 0
	if (!minimumLengthCheckbox) {
		passMinLength = 0;
	}
	if (!uppercaseCheckbox) {
		passCombUppercase = 0;
	}	
	if (!numbersCheckbox) {
		passCombDigit = 0;
	}
	if (!specialCharactersCheckbox) {
		passCombSymbol = 0;
	}
	// usagePeriodType의 경우 무기한인 경우 0, 기간제한인 경우 1
	if (usagePeriodType == 0) {
		passExpireDays = 0;
	}
	if (!expirationNotificationCheckbox) {
		passNotifyBefore = 0;
	}
	
	// 설정된 정보를 담아서 ajax로 업데이트 전달한다
	$.ajax({
		url: '/primx/fun/sys/updatePasswordSetting.do',
		type: 'POST',
		data: JSON.stringify({
			userRole: userRole,
			passMinLength: passMinLength,
			passCombUppercase: passCombUppercase,
			passCombDigit: passCombDigit,
			passCombSymbol: passCombSymbol,
			passExpireDays: passExpireDays,
			passNotifyBefore: passNotifyBefore
		}),
		contentType: 'application/json', // JSON 형식으로 전송
		success: function(response) {
			if (response.status === "success") {
				alert("비밀번호 인증 설정이 완료되었습니다.");
				closeSettingPopup();
			} else {
				alert("비밀번호 인증 설정에 실패했습니다.");
			}
		},
		error: function(xhr, status, error) {
			alert("오류 발생: " + error);
		}
	});

}

function closeSettingPopup() {
	$(".topSetPopup").addClass("hide");
}

// 공통 비밀번호 설정 정보 로드 후 리턴
async function loadCommonPasswordSettingInfo() {
	try {
		const response = await $.ajax({
			url: '/primx/fun/cmm/loadPasswordSettingInfo.do',
			type: 'GET'
		});
		console.log("loadCommonPasswordSettingInfo: ", response.data);
		return response.data; // 데이터 반환
	} catch (error) {
		alert("오류 발생: " + error);
		return null; // 오류 발생 시 null 반환
	}
}

// 비밀번호 변경 시 공통 비밀번호 설정을 검사하여 변경 가능한지 여부 리턴
/**
 * 성공시 0, 최소길이 실패시 1, 대문자 실패시 2, 숫자 실패시 3, 특수문자 실패시 4
 * @param {*} password 
 * @returns 
 */
async function checkCommonPassword(password) {
	const commonPassword = await loadCommonPasswordSettingInfo();
	
	var result = 0;
	if (!commonPassword) {
		console.error("공통 비밀번호 설정 정보를 불러오는 데 실패했습니다.");
		result = 0; // 오류 코드 반환
	}

	console.log("commonPassword: ", JSON.stringify(commonPassword));
	// 최소길이 검사
	if (password.length < commonPassword.passMinLength) {
		console.log("최소길이 실패");
		result = 1;
	}
	else if (commonPassword.passCombUppercase != 0) {
		const uppercaseCount = (password.match(/[A-Z]/g) || []).length;
		if (uppercaseCount < commonPassword.passCombUppercase) {
			console.log("대문자 실패");
			result = 2;
		}
	} else if (commonPassword.passCombDigit != 0) {
		const digitCount = (password.match(/[0-9]/g) || []).length;
		if (digitCount < commonPassword.passCombDigit) {
			console.log("숫자 실패");
			result = 3;
		}
	} else if (commonPassword.passCombSymbol != 0) {
		const symbolCount = (password.match(/[!@#$%^&*]/g) || []).length;
		if (symbolCount < commonPassword.passCombSymbol) {
			console.log("특수문자 실패");
			result = 4;
		}
	}
	
	if(result != 0) {
		var message = "비밀번호는 최소 " + commonPassword.passMinLength 
		+ "자 이상, 숫자 " + commonPassword.passCombDigit + "개 이상, 대문자 " 
		+ commonPassword.passCombUppercase + "개 이상, 특수문자 " 
		+ commonPassword.passCombSymbol + "개 이상 포함되어야 합니다.";
		alert(message);
	}

	return result;
}

// 미접속 시스템 조회 팝업
function openUnusedSystemPopup() {

	loadUnusedSystemList();
	$(".deleteUnconnectedSystemPopup").removeClass("hide");
}

// 미접속 시스템 조회 팝업 닫기
function closeUnusedSystemPopup() {
	$(".deleteUnconnectedSystemPopup").addClass("hide");
}

// 미접속 시스템 조회 목록 로드
function loadUnusedSystemList() {

	deleteUnconnectedSystemTable = initDataTables('#deleteUnconnectedSystemTable', {
		ajax: {
			url: '/primx/fun/sys/loadUnusedSystemList.do',
			type: 'POST',
			data: function(d) {
				// 페이지 번호와 페이지 크기 추가
				d.page = d.start / d.length + 1;
				d.pageSize = d.length;
			}
		},
		processing: true,
		serverSide: true,
		columns: [
			{ 
				data: null, 
				orderable: false, // 정렬 비활성화
				className: "text-center",
				render: function(data, type, row, meta) {
					if (type === "display") {
						return `<input type="checkbox" name="rowSelect" class="row-checkbox" />`;
					}
					return "";
				}
			},
			{ data: 'companyName' },
			{ data: 'deptName' },
			{ data: 'userNm' },
			{ data: 'lastKaDate', render: function(data) {
				return data ? new Date(data).toLocaleDateString() : '';
			} }
		],
		columnDefs: [
			{
				render: DataTable.render.select(),
				targets: 0
			}
		],
		select: {
			style: 'single',
			selector: 'td:first-child'
		}
	});
	
	// 단일 선택 체크박스 이벤트
	$('#deleteUnconnectedSystemTable tbody').on('change', '.row-checkbox', function() {
	    $('.row-checkbox').not(this).prop('checked', false); // 다른 체크박스 해제
	    $('#deleteUnconnectedSystemTable tbody tr').removeClass('selected'); // 모든 행에서 선택 효과 제거
	
	    if ($(this).prop('checked')) {
	        $(this).closest('tr').addClass('selected'); // 선택된 행에 배경색 적용
	    }
	});
	
	// 행 클릭 시 체크박스 선택/해제
	$('#deleteUnconnectedSystemTable tbody').on('click', 'tr', function(e) {
	    // 체크박스를 클릭한 경우에는 이 이벤트가 중복 실행되지 않도록 방지
	    if ($(e.target).is('.row-checkbox') || $(e.target).is('input')) {
	        return;
	    }
	
	    let checkbox = $(this).find('.row-checkbox');
	    let isChecked = checkbox.prop('checked');
	
	    // 모든 체크박스 해제
	    $('.row-checkbox').prop('checked', false);
	    $('#deleteUnconnectedSystemTable tbody tr').removeClass('selected');
	
	    // 현재 클릭한 행의 체크박스 상태를 반대로 토글
	    checkbox.prop('checked', !isChecked);
	    
	    if (!isChecked) {
	        $(this).addClass('selected'); // 선택된 행 스타일 적용
	    }
	});
}

// 미접속 시스템 조회 팝업 테이블 삭제
function deleteUnusedSystem() {
	// 선택된 row의 데이터 정보를 가져옴
	var selectedRow = deleteUnconnectedSystemTable.rows({ selected: true }).data();

	// selectedRow의 내용을 개별적으로 출력
	selectedRow.each(function(row) {
		console.log('Row data:', row); // 각 행의 데이터 출력
	});

	$.ajax({
		url: '/primx/fun/sys/deleteSystem.do',
		type: 'POST',
		data: {
			seqNo: selectedRow[0].seqNo
		},
		success: function(response) {
			console.log(response);
			alert('시스템이 삭제 처리 되었습니다.');
			loadUnusedSystemList();
		},
		error: function(error) {
			console.log(error);
			alert('시스템 삭제 처리 중 오류가 발생했습니다.');
		}
	});

}






// 로그아웃 함수
function logoutMain() {
	alert("로그아웃합니다.");
	logout();
}

// 로그아웃 함수
function logout() {
	window.location.href = "/uat/uia/egovLoginUsr.do";
}

// 숫자만 입력 가능하도록 제한하는 함수
function validateNumberInput(input) {
    const value = input.value;
    if (!/^\d+$/.test(value)) {
        const label = $(input).prev('label').text().trim();
        alert(label + "은(는) 숫자만 입력 가능합니다.");
        $(input).focus();
        return false;
    }
    return true;
}

// 숫자 입력 필드에 이벤트 리스너 추가
$(function() {
    // 숫자만 입력 가능한 필드들에 대해 이벤트 리스너 추가
    $('#term, #keepalive, #logPush, #statePush, #passMinLength, #passCombUppercase, #passCombDigit, #passCombSymbol, #passExpireDays, #passNotifyBefore').on('input', function(e) {
        const value = $(this).val();
        if (!/^\d*$/.test(value)) {
            const label = $(this).prev('label').text().trim();
            alert(label + "은(는) 숫자만 입력 가능합니다.");
            $(this).val(value.replace(/[^0-9]/g, ''));
            $(this).focus();
        }
    });
});

$(document).ready(function() {
    // 운영 이력 데이터 삭제 체크박스 이벤트 핸들러
    $('#operationHistoryDataDeleteCheckbox').on('change', function() {
        const isChecked = $(this).is(':checked');
        $('#periodSelect, #startDatePicker, #endDatePicker, #operationHour, #operationMinute').prop('disabled', !isChecked);
        
        // 기간지정이 아닌 경우 datepicker 비활성화
        if ($('#periodSelect').val() !== '') {
            $('#startDatePicker, #endDatePicker').prop('disabled', true);
        }
    });

    // 제품 이력 데이터 삭제 체크박스 이벤트 핸들러
    $('#productHistoryDataDeleteCheckbox').on('change', function() {
        const isChecked = $(this).is(':checked');
        $('#periodSelect2, #startDatePicker2, #endDatePicker2, #productHour, #productMinute').prop('disabled', !isChecked);
        
        // 기간지정이 아닌 경우 datepicker 비활성화
        if ($('#periodSelect2').val() !== '') {
            $('#startDatePicker2, #endDatePicker2').prop('disabled', true);
        }
    });

    // 운영 이력 데이터 삭제 기간 선택 이벤트 핸들러
    $('#periodSelect').on('change', function() {
        const isChecked = $('#operationHistoryDataDeleteCheckbox').is(':checked');
        const selectedValue = $(this).val();
        
        if (selectedValue === '') {
            // 기간지정 선택 시 datepicker 활성화
            $('#startDatePicker, #endDatePicker').prop('disabled', false);
        } else {
            // 다른 옵션 선택 시 datepicker 비활성화
            $('#startDatePicker, #endDatePicker').prop('disabled', true);
        }
    });

    // 제품 이력 데이터 삭제 기간 선택 이벤트 핸들러
    $('#periodSelect2').on('change', function() {
        const isChecked = $('#productHistoryDataDeleteCheckbox').is(':checked');
        const selectedValue = $(this).val();
        
        if (selectedValue === '') {
            // 기간지정 선택 시 datepicker 활성화
            $('#startDatePicker2, #endDatePicker2').prop('disabled', false);
        } else {
            // 다른 옵션 선택 시 datepicker 비활성화
            $('#startDatePicker2, #endDatePicker2').prop('disabled', true);
        }
    });

    // 비밀번호 설정 체크박스 이벤트 핸들러
    function toggleInputDisabled(checkboxId, inputId) {
        const checkbox = $(`#${checkboxId}`);
        const input = $(`#${inputId}`);
        
        // 체크박스 상태 변경 시 이벤트
        checkbox.on('change', function() {
            input.prop('disabled', !$(this).is(':checked'));
        });
    }

    // 각 체크박스와 입력 필드 쌍에 대해 토글 함수 적용
    toggleInputDisabled('minimumLengthCheckbox', 'passMinLength');
    toggleInputDisabled('uppercaseCheckbox', 'passCombUppercase');
    toggleInputDisabled('numbersCheckbox', 'passCombDigit');
    toggleInputDisabled('specialCharactersCheckbox', 'passCombSymbol');
    toggleInputDisabled('expirationNotificationCheckbox', 'passNotifyBefore');
    
    // 사용기간 타입 변경 이벤트
    $('#usagePeriodType').on('change', function() {
        const isLimited = $(this).val() === '1';
        $('#passExpireDays').prop('disabled', !isLimited);
    });
});

// 공통기능 byte 단위변환
function convertByteUnit(bytes) {
    const units = ['B', 'KB', 'MB', 'GB', 'TB'];
    let index = 0;
    while (bytes >= 1024 && index < units.length - 1) {
        bytes /= 1024;
        index++;
    }
    return bytes.toFixed(2) + ' ' + units[index];
}

// 공통기능 byte 단위변환 (쉼표 포함, GB까지만 표시)
// size: 변환할 크기
// unit: 입력 단위 ('B', 'KB', 'MB', 'GB')
function convertByteUnitWithComma(size, unit = 'B') {
    const units = ['B', 'KB', 'MB', 'GB'];
    let index = 0;
    
    // size를 숫자로 변환
    size = Number(size);
    
    // 입력 단위에 따라 바이트로 변환
    switch(unit) {
        case 'KB':
            size = size * 1024;
            break;
        case 'MB':
            size = size * 1024 * 1024;
            break;
        case 'GB':
            size = size * 1024 * 1024 * 1024;
            break;
    }
    
    while (size >= 1024 && index < units.length - 1) {
        size /= 1024;
        index++;
    }
    // 숫자에 쉼표 추가
    const formattedNumber = size.toFixed(2).replace(/\B(?=(\d{3})+(?!\d))/g, ",");
    return formattedNumber + ' ' + units[index];
}


