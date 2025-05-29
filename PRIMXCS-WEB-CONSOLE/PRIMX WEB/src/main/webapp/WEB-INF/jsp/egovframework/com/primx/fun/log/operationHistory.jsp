<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.operationHistory">PRIMX | 운영 이력</title>
<form class="operationHistory" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.operationHistory"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.history"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.operationHistory"></span>
			</h2>
		</div>
		<div class="searchInnerWrap">
			<div class="searchBox">
				<input type="text" class="txtBox searchInput" readonly data-i18n-placeholder="search.searchTit" name="searchKeyword" value="${searchVO.searchKeyword}">
			</div>
			<div class="dropMenu">
				<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
				<ul>
					<li class="csv"><button type="button" id="operationHistoryCsvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
				</ul>
			</div>
		</div>
	</div>
	<div class="tableWrap">
		<table class="dataTable checkDataTable" id="dataTable">
			<thead>
				<tr>
					<th width="2%"></th>
					<th width="5%" data-i18n="operationHistory.grid.date">일자</th>
					<th width="5%" data-i18n="operationHistory.grid.time">시각</th>
					<th width="5%" data-i18n="operationHistory.grid.category">분류</th>
					<th width="10%" data-i18n="operationHistory.grid.kind">종류</th>
					<th width="42%" data-i18n="operationHistory.grid.content">내용</th>
					<th width="10%" data-i18n="operationHistory.grid.systemName">시스템명</th>
					<th width="10%" data-i18n="operationHistory.grid.deptName">부서명</th>
					<th width="5%" data-i18n="operationHistory.grid.userName">사용자명</th>
				</tr>
			</thead>
			<tbody>

			</tbody>
		</table>
	</div>
</form>

<!-- 운영이력 상세 보기 -->
<section class="popup operationHistoryViewPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="operationHistory.operationHistoryViewPopup.popTitle">정보</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line w2">
						<div class="item">
							<em data-i18n="operationHistory.operationHistoryViewPopup.dateTime">일시</em>
							<div class=""></div>
						</div>
						<div class="item">
							<em data-i18n="operationHistory.operationHistoryViewPopup.category">분류</em>
							<div class=""></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="operationHistory.operationHistoryViewPopup.kind">종류</em>
							<div class=""></div>
						</div>
					</li>
				</ul>
				<ul class="setWrap setWrapScroll mb00">
					<li class="line">
						<div class="item">
							<em data-i18n="operationHistory.operationHistoryViewPopup.systemName">시스템명</em>
							<div></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="operationHistory.operationHistoryViewPopup.userName">사용자명</em>
							<div></div>
						</div>
					</li>
					<!-- <li class="line">
						<div class="item">
							<em data-i18n="operationHistory.operationHistoryViewPopup.workerName">작업자명</em>
							<div></div>
						</div>
					</li> -->
					<li class="line">
						<div class="item">
							<em data-i18n="operationHistory.operationHistoryViewPopup.content">내용</em>
							<div class="contentArea"></div>
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.operationHistoryViewPopup').addClass('hide');">닫기</button>
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
							<label data-i18n="search.searchSubjects.category">분류</label>
							<div class="selectBox">
								<select class="drop" name="searchCategory">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="연동" data-i18n="search.option.integration">연동</option>
									<option value="시스템" data-i18n="search.option.system">시스템</option>
									<option value="접속" data-i18n="search.option.access">접속</option>
									<option value="정책" data-i18n="search.option.policy">정책</option>
									<option value="관리자" data-i18n="search.option.admin">관리자</option>
									<option value="사용자" data-i18n="search.option.user">사용자</option>
									<option value="업데이트" data-i18n="search.option.update">업데이트</option>
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
									<option value="인사 연동" data-i18n="search.option.hrIntegration">인사 연동</option>
									<option value="미접속 시스템 삭제" data-i18n="search.option.deleteInactiveSystem">미접속 시스템 삭제</option>
									<option value="시스템 사용자 변경" data-i18n="search.option.changeSystemUser">시스템 사용자 변경</option>
									<option value="에이전트 삭제" data-i18n="search.option.deleteAgent">에이전트 삭제</option>
									<option value="관리자 로그인" data-i18n="search.option.adminLogin">관리자 로그인</option>
									<option value="사용자 로그인" data-i18n="search.option.userLogin">사용자 로그인</option>
									<option value="제품 정책 변경" data-i18n="search.option.productPolicyChange">제품 정책 변경</option>
									<option value="제품 정책 적용" data-i18n="search.option.applyProductPolicy">제품 정책 적용</option>
									<option value="시스템 종류 변경" data-i18n="search.option.changeSystemType">시스템 종류 변경</option>
									<option value="PRIMX Agent 정책 적용" data-i18n="search.option.applyPRIMXAgentPolicy">PRIMX Agent 정책 적용</option>
									<option value="관리자 추가" data-i18n="search.option.addAdmin">관리자 추가</option>
									<option value="관리자 삭제" data-i18n="search.option.deleteAdmin">관리자 삭제</option>
									<option value="수동 사용자 추가" data-i18n="search.option.manualUserAdd">수동 사용자 추가</option>
									<option value="수동 사용자 삭제" data-i18n="search.option.manualUserDelete">수동 사용자 삭제</option>
									<option value="PRIMX Agent 업데이트" data-i18n="search.option.updatePRIMXAgent">PRIMX Agent 업데이트</option>
									<option value="제품 업데이트" data-i18n="search.option.updateProduct">제품 업데이트</option>
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
							<input type="text" class="txtBox w330" name="searchKeyword" id="searchKeyword" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchOperationHistory();}" />
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" onclick="searchOperationHistory();" data-i18n="button.search">검색</button>
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.searchAreaPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/log/operationHistory.js' />"></script>