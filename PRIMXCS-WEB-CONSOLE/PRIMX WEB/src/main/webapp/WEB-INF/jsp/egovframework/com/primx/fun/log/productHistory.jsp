<%@ page pageEncoding="utf-8"%>

<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="ui" uri="http://egovframework.gov/ctl/ui"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags" %>
<title data-i18n="webTitle.productHistory">PRIMX | 제품 이력</title>
<form class="productHistory" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.productHistory"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.history"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.productHistory"></span>
			</h2>
		</div>
		<div class="searchInnerWrap">
			<div class="searchBox">
				<input type="text" class="txtBox searchInput" readonly data-i18n-placeholder="search.searchTit" name="searchKeyword" value="${searchVO.searchKeyword}">
			</div>
			<div class="dropMenu">
				<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
				<ul>
					<li class="csv"><button type="button" id="productHistoryCsvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
				</ul>
			</div>
		</div>
	</div>
	<div class="tableWrap">
		<table class="dataTable checkDataTable" id="dataTable">
			<thead>
				<tr>
					<th width="2%"></th>
					<th width="5%" data-i18n="productHistory.grid.date">일자</th>
					<th width="5%" data-i18n="productHistory.grid.time">시각</th>
					<th data-i18n="productHistory.grid.product">제품</th>
					<th data-i18n="productHistory.grid.level">레벨</th>
					<th width="10%" data-i18n="productHistory.grid.kind">종류</th>
					<th width="42%" data-i18n="productHistory.grid.content">내용</th>
					<th width="10%" data-i18n="productHistory.grid.systemName">시스템명</th>
					<th width="10%" data-i18n="productHistory.grid.deptName">부서명</th>
					<th width="5%" data-i18n="productHistory.grid.userName">사용자명</th>
				</tr>
			</thead>
			<tbody>
				<!-- 데이터는 DataTables에 의해 동적으로 채워집니다 -->
			</tbody>
		</table>
	</div>

</form>

<!-- 제품이력 상세 보기 -->
<section class="popup productHistoryViewPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="productHistory.productHistoryViewPopup.popTitle">정보</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line w2">
						<div class="item">
							<em data-i18n="productHistory.productHistoryViewPopup.dateTime">일시</em>
							<div class=""></div>
						</div>
						<div class="item">
							<em data-i18n="productHistory.productHistoryViewPopup.level">레벨</em>
							<div class=""></div>
						</div>
					</li>
					<li class="line w2">
						<div class="item">
							<em data-i18n="productHistory.productHistoryViewPopup.product">제품</em>
							<div class=""></div>
						</div>
						<div class="item">
							<em data-i18n="productHistory.productHistoryViewPopup.kind">종류</em>
							<div class=""></div>
						</div>
					</li>
				</ul>
				<ul class="setWrap setWrapScroll mb00">
					<li class="line">
						<div class="item">
							<em data-i18n="productHistory.productHistoryViewPopup.systemName">시스템명</em>
							<div></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="productHistory.productHistoryViewPopup.userName">사용자명</em>
							<div></div>
						</div>
					</li>
					<!-- <li class="line">
						<div class="item">
							<em data-i18n="productHistory.productHistoryViewPopup.workerName">작업자명</em>
							<div></div>
						</div>
					</li> -->
					<li class="line">
						<div class="item">
							<em data-i18n="productHistory.productHistoryViewPopup.content">내용</em>
							<div class="contentArea"></div>
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.productHistoryViewPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 검색 팝업 -->
<section class="popup searchAreaPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="search.searchTit">검색</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="search_wrap">
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.period">기간</label>
							<div class="selectBox">
								<select class="drop" name="dateSelect" id="dateSelect" data-label="기간">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="today" data-i18n="search.option.today">오늘</option>
									<option value="this-week" data-i18n="search.option.week">이번주</option>
									<option value="this-month" data-i18n="search.option.month">이번 달</option>
									<option value="custom" data-i18n="search.option.specifyPeriod">기간지정</option>
								</select>
							</div>
							<input type="text" class="datepicker datePickerInner txtBox start-date" name="searchStartDate" data-i18n-placeholder="datepicker.startDate" readonly>
							<input type="text" class="datepicker datePickerInner txtBox end-date" name="searchEndDate" data-i18n-placeholder="datepicker.endDate" readonly>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.level">레벨</label>
							<div class="selectBox">
								<select class="drop" name="searchLevel">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="INFO">INFO</option>
									<option value="WARNING">WARNING</option>
									<option value="ERROR">ERROR</option>
								</select>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.product">제품</label>
							<div class="selectBox">
								<select class="drop" name="searchProduct">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="Agent">Agent</option>
									<!-- <option value="CRYHOD">CRYHOD</option>
									<option value="ORIZON">ORIZON</option> -->
									<option value="ZONECENTRAL">ZONECENTRAL</option>
								</select>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.kind">종류</label>
							<div class="selectBox">
								<select class="drop" name="searchType">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="실행 오류" data-i18n="search.option.executionError">실행 오류</option>
									<option value="명령 실행" data-i18n="search.option.commandExecution">명령 실행</option>
									<option value="엑세스 리스트 색인 생성" data-i18n="search.option.accessListIndexCreation">엑세스 리스트 색인 생성</option>
									<option value="엑세스 리스트 폴더 생성" data-i18n="search.option.accessListFolderCreation">엑세스 리스트 폴더 생성</option>
									<option value="파일 암복호화" data-i18n="search.option.fileEncryptionDecryption">파일 암복호화</option>
									<option value="폴더 암복호화" data-i18n="search.option.folderEncryptionDecryption">폴더 암복호화</option>
									<option value="암호 영역 검사" data-i18n="search.option.cryptoAreaCheck">암호 영역 검사</option>
									<option value="암호 영역 접근" data-i18n="search.option.cryptoAreaAccess">암호 영역 접근</option>
									<option value="영역 암호화" data-i18n="search.option.areaEncryption">영역 암호화</option>
									<option value="암호 파일 접근" data-i18n="search.option.cryptoFileAccess">암호 파일 접근</option>
									<option value="공유 폴더 갱신" data-i18n="search.option.sharedFolderUpdate">공유 폴더 갱신</option>
									<option value="시스템 동작" data-i18n="search.option.systemOperation">시스템 동작</option>
									<option value="파일 변경" data-i18n="search.option.fileChange">파일 변경</option>
									<option value="영역 업데이트" data-i18n="search.option.areaUpdate">영역 업데이트</option>
									<option value="접근 목록 파일 업데이트" data-i18n="search.option.accessListFileUpdate">접근 목록 파일 업데이트</option>
									<option value="접근 목록 파일 접근" data-i18n="search.option.accessListFileAccess">접근 목록 파일 접근</option>
									<option value="세션 업데이트" data-i18n="search.option.sessionUpdate">세션 업데이트</option>
									<option value="내보내기" data-i18n="search.option.export">내보내기</option>
									<option value="인증서 파일 접근" data-i18n="search.option.certificateFileAccess">인증서 파일 접근</option>
									<option value="SSO 동작" data-i18n="search.option.SSOOperation">SSO 동작</option>
									<option value="접근 키 업데이트" data-i18n="search.option.accessKeyUpdate">접근 키 업데이트</option>
									<option value="접근 키 접근" data-i18n="search.option.accessKeyAccess">접근 키 접근</option>
									<option value="복구 동작" data-i18n="search.option.recoveryOperation">복구 동작</option>
									<option value="그룹 정책 업데이트" data-i18n="search.option.groupPolicyUpdate">그룹 정책 업데이트</option>
									<option value="컨테이너" data-i18n="search.option.container">컨테이너</option>
									<option value="실행 경고" data-i18n="search.option.executionWarning">실행 경고</option>
								</select>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.condition">조건</label>
							<div class="selectBox">
								<select class="drop" name="searchCondition">
									<option value="all" data-i18n="search.option.all">전체</option>
									<option value="computerName" data-i18n="search.option.systemName">시스템명</option>
									<option value="userName" data-i18n="search.option.userName">사용자명</option>
									<option value="deptName" data-i18n="search.option.departmentName">부서명</option>
								</select>
							</div>
							<input type="text" class="txtBox w330" name="searchKeyword" id="searchKeyword" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchProductHistory();}" />
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" onclick="searchProductHistory()" data-i18n="button.search">검색</button>
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.searchAreaPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/log/productHistory.js' />"></script>