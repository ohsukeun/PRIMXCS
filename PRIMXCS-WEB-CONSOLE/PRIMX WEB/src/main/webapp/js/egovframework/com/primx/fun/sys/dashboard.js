let productLogDataTable;
let policyDataTable;
/* 커스텀 드롭 메뉴 온 오프 */
function dropMenuHamberger() {
	/* 드롭 메뉴 온 오프 */
	$(".dropMenu_dashboard > .selectMenu").toggleClass("on");
	// 드롭 메뉴 영역 외부 클릭 시 닫기
    $("body").on("click", function (e) {
        if (!$(e.target).closest(".dropMenu_dashboard").length) {
            $(".dropMenu_dashboard > .selectMenu").removeClass("on");
        }
    });
}
    
function pageInit(){

	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();
	
	// 정책 현황
	getPolicyStatus();
	initPolicyDataTable();

	// 라이선스 현황
	getLicenseStatus();
	// 시스템 현황
	getSystemStatus();
	// 결재 현황
	getRequestApprovalStatus();

	// 이동파일 현황
	getMoveFileStatus();
	// 이동파일 대시보드
	switchMoveFileDashboard();
	
	// 제품 이력
	initProductLogDashboard();
	// 제품 이력 체크박스 이벤트 핸들러
	setCheckboxEvent();
	// 제품 이력 테이블
	initProductLogDataTable();
}

function initProductLogDataTable() {
	// 기존 테이블이 있다면 제거
	if (productLogDataTable) {
		productLogDataTable.destroy();
		productLogDataTable = null;
	}

	productLogDataTable = initDataTables('#productLogDataTable',{
		processing: true,
		serverSide: true,
		destroy: true,  /* 기존 인스턴스 제거 옵션 추가 */
		ajax: {
			url: '/primx/fun/sys/dashboard/productLog/list.do',
			type: 'POST'
		},
		columns: [
			{ data: 'logDate' },
			{ data: 'logTime' },
			{ data: 'logLevel' },
			{ data: 'logName' },
			{ data: 'userName' }
		],
        layout: {
			topStart: null,
			topEnd: null,
			bottomStart: null,
			bottomEnd: null
		}
	});
	
}

function initPolicyBar(totalDefaultPolicyCnt, unappliedDefaultPolicyCnt, appliedCustomPolicyCnt, unappliedCustomPolicyCnt) {
	
	// 기본적용
	const basicApplied = totalDefaultPolicyCnt;
	// 기본미적용
	const basicNotApplied = unappliedDefaultPolicyCnt;
	// 개별적용
	const indivApplied = appliedCustomPolicyCnt ;
	// 개별미적용
	const indivNotApplied = unappliedCustomPolicyCnt;
	
	// 제품 정책 바
	const policyBarTotal = basicApplied + basicNotApplied + indivApplied + indivNotApplied;
	const policyBarData = [
		{ type: 'dashboard.default', status: 'dashboard.applied', value: basicApplied, className: 'basic-applied' },
		{ type: 'dashboard.default', status: 'dashboard.unapplied', value: basicNotApplied, className: 'basic-notapplied' },
		{ type: 'dashboard.individual', status: 'dashboard.applied', value: indivApplied, className: 'indiv-applied' },
		{ type: 'dashboard.individual', status: 'dashboard.unapplied', value: indivNotApplied, className: 'indiv-notapplied' }
	];
	console.log(policyBarData);

	const policyBar = document.getElementById('policy-bar');
	policyBar.innerHTML = '';

	policyBarData.forEach(item => {
		const widthPercent = (item.value / policyBarTotal) * 100;
		const div = document.createElement('div');
		div.className = `policy-segment ${item.className}`;
		div.style.width = widthPercent + '%';
		div.innerHTML = `
		<div class="policy-segment-inner">
			<div class="policy-value">${item.value}</div>
			<div class="policy-type" data-i18n="${item.type}"></div>
			<div class="policy-status" data-i18n="${item.status}"></div>
		</div>
		`;
		policyBar.appendChild(div);
	});
	// 다국어 함수 호출출
	applyTranslations();
}

function getPolicyStatus() {
	$.ajax({
		url: '/primx/fun/sys/dashboard/policyStatus.do',
		type: 'POST',
		dataType: 'json',
		success: function(data) {
			console.log(data);
			initPolicyBar(data.data.totalDefaultPolicyCnt, data.data.unappliedDefaultPolicyCnt, data.data.appliedCustomPolicyCnt, data.data.unappliedCustomPolicyCnt);
		},
		error: function(xhr, status, error) {
			console.log(xhr);
			console.log(status);
			console.log(error);
		}
	});
}

function initPolicyDataTable() {
	// 기존 테이블이 있다면 제거
	if (policyDataTable) {
		policyDataTable.destroy();
		policyDataTable = null;
	}

	policyDataTable = initDataTables('#policyDataTable',{
		processing: true,
		serverSide: true,
		destroy: true,  /* 기존 인스턴스 제거 옵션 추가 */
		ajax: {
			url: '/primx/fun/sys/dashboard/individualPolicyList.do',
			type: 'POST'
		},
		columns: [
			{ data: 'policyCode' },
			{ data: 'policyName' },
			{ data: 'systemMapCount' },
			{ data: 'usedSystemMapCount' }
		],
        layout: {
			topStart: null,
			topEnd: null,
			bottomStart: null,
			bottomEnd: null
		}
	});
	
}


/* 정책 툴팁 */
let isTooltipHovered = false;

// 정책 segment에 마우스 진입
$(document).on('mouseenter', '.policy-segment.indiv-applied', function(e) {
    const $segment = $(this);
    const $tooltip = $('.policy-tooltip');

    // 위치 계산
    const rect = this.getBoundingClientRect();
    const tooltipWidth = $tooltip.outerWidth();
    const tooltipHeight = $tooltip.outerHeight();

    // 중앙 좌표 계산 (viewport 기준)
	const left = rect.left + (rect.width / 2);
	const top = rect.top + (rect.height / 2);

    // 툴팁 위치 및 표시
    $tooltip.css({
        left: left + 'px',
        top: top + 'px',
        display: 'block'
    });
});

// 정책 segment에서 마우스가 벗어날 때
$(document).on('mouseleave', '.policy-segment.indiv-applied', function() {
    setTimeout(function() {
        if (!isTooltipHovered) {
            $('.policy-tooltip').css('display', 'none');
        }
    }, 50); // 50ms 딜레이로 자연스러운 이동 허용
});

// 툴팁에 마우스 진입
$(document).on('mouseenter', '.policy-tooltip', function() {
    isTooltipHovered = true;
});

// 툴팁에서 마우스가 벗어날 때
$(document).on('mouseleave', '.policy-tooltip', function() {
    isTooltipHovered = false;
    $('.policy-tooltip').css('display', 'none');
});

function getLicenseStatus() {
	$.ajax({
		url: '/primx/fun/sys/dashboard/licenseStatus.do',
		type: 'POST',
		dataType: 'json',
		success: function(data) {
			console.log(data);
			initLicenseChart(data.data.totalUsedCnt, data.data.zcLicenseCnt);
		},
		error: function(xhr, status, error) {
			console.log(xhr);
			console.log(status);
			console.log(error);
		}
	});
}

// 이동파일 대시보드 라디오버튼 선택시 이벤트 추가 
function switchMoveFileDashboard() {
	// radio 버튼 이벤트 추가
	document.querySelectorAll('input[name="period"]').forEach(radio => {
		radio.addEventListener('change', function () {
			renderFileMoveChart(this.value);
		});
	});
}


function getMoveFileStatus() {
	console.log('getMoveFileStatus');
	// radio 버튼 값 가져오기
	const period = document.querySelector('input[name="period"]:checked').value;
	console.log(period);
	$.ajax({
		url: '/primx/fun/sys/dashboard/moveFileStatus.do',
		type: 'POST',
		dataType: 'json',
		success: function(data) {
			console.log(data);
			initMoveFileChart(data.data);
		},
		error: function(xhr, status, error) {
			console.log(xhr);
			console.log(status);
			console.log(error);
		}
	});
}

function getSystemStatus() {
	$.ajax({
		url: '/primx/fun/sys/dashboard/systemStatus.do',
		type: 'POST',
		dataType: 'json',
		success: function(data) {
			console.log(data);
			$('#cpuStat').text(data.data.cpuStat);
			$('#memStat').text(data.data.memStat);
			$('#memTotal').text(data.data.memTotal);
			$('#diskStat').text(data.data.diskStat);
			$('#diskTotal').text(data.data.diskTotal);
			$('#commonPcCount').text(data.data.commonPcCount);
			$('#userPcCount').text(data.data.userPcCount);
			$('#notConnectedPcCount').text(data.data.notConnectedPcCount);

            // cpuStat이 85 이상이면 .cpuLi에 redColor 클래스 추가, 아니면 제거
            var cpuValue = parseFloat(data.data.cpuStat);
            if (!isNaN(cpuValue) && cpuValue >= 85) {
                $('.cpuLi').addClass('redColor');
            } else {
                $('.cpuLi').removeClass('redColor');
            }
		},
		error: function(xhr, status, error) {
			console.log(xhr);
			console.log(status);
			console.log(error);
		}
	});
}

function getRequestApprovalStatus() {
	$.ajax({
		url: '/primx/fun/sys/dashboard/requestApprovalStatus.do',
		type: 'POST',
		dataType: 'json',
		success: function(data) {
			console.log(data);
			$('#requestCount').text(data.data.requestCount);
			$('#userRegistCount').text(data.data.userRegistCount);
			$('#accountExtendCount').text(data.data.accountExtendCount);
			$('#moveFileCount').text(data.data.moveFileCount);
			$('#approvalCount').text(data.data.approvalCount);
			$('#approvalAllowCount').text(data.data.approvalAllowCount);
			$('#rejectCount').text(data.data.rejectCount);
		},	
		error: function(xhr, status, error) {
			console.log(xhr);
			console.log(status);
			console.log(error);
		}
	});
}	

/* 이동파일 대시보드 */
var chartData = {
	day: {
		labels: ['2025.01.27'],
		created: [0],
		used: [0],
		expired: [0]
	},
	week: {
		labels: ['2025.01.20 - 2025.01.27'],
		created: [0],
		used: [0],
		expired: [0]
	},
	month: {
		labels: ['2025.01'],
		created: [0],
		used: [0],
		expired: [0]
	}
};

function initMoveFileChart(data) {
	console.log(data);
	
	/* 이동파일 대시보드 */
	chartData = {
		day: {
			labels: [data.dayLabel],
			created: [data.dayCreatedCnt],
			used: [data.dayUsedCnt],
			expired: [data.dayExpiredCnt]
		},
		week: {
			labels: [data.dayLabel + ' - ' + data.weekLabel],
			created: [data.weekCreatedCnt],
			used: [data.weekUsedCnt],
			expired: [data.weekExpiredCnt]
		},
		month: {
			labels: [data.dayLabel + ' - ' + data.monthLabel],
			created: [data.monthCreatedCnt],
			used: [data.monthUsedCnt],
			expired: [data.monthExpiredCnt]
		}
	};

	// DOMContentLoaded, load 모두에서 시도
	document.addEventListener('DOMContentLoaded', () => safeRenderFileMoveChart('day'));
	window.addEventListener('load', () => safeRenderFileMoveChart('day'));

	// ResizeObserver로 chartWrap 크기 변동 감지 시 재렌더링
	const chartWrap = document.querySelector('.chartWrap');
		if (chartWrap) {
		const observer = new ResizeObserver(() => {
			safeRenderFileMoveChart('day');
		});
		observer.observe(chartWrap);
	}
}

function adjustCenterTextSize() {
    const canvas = document.getElementById('licenseChart');
    const centerText = document.getElementById('centerText');
    const chartBg = document.querySelector('.chart-bg');
    
    if (canvas && centerText && chartBg) {
        const canvasWidth = canvas.width;
        const canvasHeight = canvas.height;
        
        // canvas 크기의 80%로 centerText 크기 설정
        const centerTextSize = Math.min(canvasWidth, canvasHeight) * 0.8;
        // chart-bg는 centerText보다 10% 작게 설정
        const chartBgSize = centerTextSize * 0.9;
        
        // centerText 크기 설정
        centerText.style.width = `${centerTextSize}px`;
        centerText.style.height = `${centerTextSize}px`;
        centerText.style.fontSize = `${centerTextSize * 0.2}px`;
        
        // chart-bg 크기 설정
        chartBg.style.width = `${chartBgSize}px`;
        chartBg.style.height = `${chartBgSize}px`;
    }

}

function initLicenseChart(totalUsedCnt, zcLicenseCnt) {

	var usedCnt = totalUsedCnt;
	var unusedCnt = zcLicenseCnt - totalUsedCnt;

	// 차트 초기화 코드 수정
	const data = {
		labels: ['미사용 라이선스', '사용된 라이선스'],
		datasets: [{
			data: [unusedCnt, usedCnt],
			backgroundColor: [
				'#26225e',
				'#ffb32c'
			],
			borderWidth: 0,
			borderRadius: 100,
			hoverOffset: 4,
			cutout: '80%'
		}]
	};
	
	const config = {
		type: 'doughnut',
		data: data,
		options: {
			plugins: {
				legend: { display: false },
				tooltip: { enabled: true }
			},
			cutout: '80%',
			responsive: true,
			maintainAspectRatio: false,
			onResize: function(chart, size) {
				adjustCenterTextSize();
			}
		}
	};

	$('#usedCnt').text(usedCnt);
	$('#totalCnt2').text('/ ' + zcLicenseCnt);
	$('#notUsedCnt').text(unusedCnt);
	$('#totalCnt1').text('/ ' + zcLicenseCnt);
	
	// 미사용 % 계산
	var percent = (unusedCnt / zcLicenseCnt) * 100;
	// null 체크
	if (isNaN(percent)) {
		percent = 0;
	}
	$('#centerText').text(percent.toFixed(1) + '%');

	const ctx = document.getElementById('licenseChart').getContext('2d');
	const chart = new Chart(ctx, config);
	
	// 초기 로드 시 크기 조정
	window.addEventListener('load', adjustCenterTextSize);
	// 리사이즈 이벤트 발생 시 크기 조정
	window.addEventListener('resize', adjustCenterTextSize);
}

let fileMoveChart;

function safeRenderFileMoveChart(period = 'day') {
	const chartWrap = document.querySelector('.chartWrap');
	if (!chartWrap) {
		// chartWrap이 아직 없으면 100ms 후 재시도
		setTimeout(() => safeRenderFileMoveChart(period), 100);
		return;
	}
	const rect = chartWrap.getBoundingClientRect();
	if (rect.width > 0 && rect.height > 0) {
		renderFileMoveChart(period);
	} else {
		// 크기가 아직 0이면 100ms 후 재시도
		setTimeout(() => safeRenderFileMoveChart(period), 100);
	}
}

function renderFileMoveChart(period = 'day') {

	const canvas = document.getElementById('fileMoveChart');
	const ctx = canvas.getContext('2d');
	const data = chartData[period];

	const maxY = Math.max(...data.created, ...data.used, ...data.expired) + 1;

	if (fileMoveChart) {
		fileMoveChart.destroy();
	}

	fileMoveChart = new Chart(ctx, {
		type: 'bar',
		data: {
		labels: data.labels,
		datasets: [
			{
			label: '생성',
			data: data.created,
			backgroundColor: '#ffb32c',
			borderRadius: 100,
			barPercentage: 0.9,
			categoryPercentage: 0.6
			},
			{
			label: '사용',
			data: data.used,
			backgroundColor: '#26225e',
			borderRadius: 100,
			barPercentage: 0.9,
			categoryPercentage: 0.6
			},
			{
			label: '만료',
			data: data.expired,
			backgroundColor: '#bdbdbd',
			borderRadius: 100,
			barPercentage: 0.9,
			categoryPercentage: 0.6
			}
		]
		},
		options: {
		responsive: true,
		maintainAspectRatio: false,
		plugins: {
			legend: { display: false }
		},
		scales: {
			x: {
			grid: { display: false },
			ticks: {
				color: '#26225e',
				font: { weight: 700, size: 16 }
			}
			},
			y: {
			beginAtZero: true,
			min: 0,
			max: maxY,
			grid: { color: '#ffb32c', lineWidth: 1 },
			ticks: {
				color: '#ffb32c',
				font: { weight: 700, size: 16 }
			}
			}
		}
		}
	});
}

function initProductLogDashboard() {
	// 로그 필터 조회
	$.ajax({
		url: '/primx/fun/sys/dashboard/getLogFilter.do',
		type: 'POST',
		dataType: 'json',
		success: function(data) {
			console.log(data);
			setProductLogCheckbox(data.data);
		},
		error: function(xhr, status, error) {
			console.log(xhr);
			console.log(status);
			console.log(error);
		}
	});
}

function setProductLogCheckbox(codeList) {
	
	console.log("setProductLogCheckbox");
	// codeList 에 해당하는 값을 가진 체크박스 체크
	$('.dashItem input[type="checkbox"]').each(function() {
		if (codeList.includes($(this).val())) {
			$(this).prop('checked', true);
		}
	});
}

function setCheckboxEvent() {
	// dashItem의 체크박스 이벤트 핸들러
	$('.dashItem input[type="checkbox"]').on('change', function() {
		// 체크 여부 확인
		if ($(this).is(':checked')) {
			updateProductLogStatus($(this).val(), true);
		} else {
			updateProductLogStatus($(this).val(), false);
		}
	});
}


function updateProductLogStatus(code, checked) {

	$.ajax({
		url: '/primx/fun/sys/dashboard/updateProductLogStatus.do',
		type: 'POST',
		dataType: 'json',
		data: { code: code, checked: checked },
		success: function(data) {
			console.log(data);
			initProductLogDataTable();
		},
		error: function(xhr, status, error) {
			console.log(xhr);
			console.log(status);
			console.log(error);
		}
	});
}

function refreshDashboard() {
	window.location.reload();
}
