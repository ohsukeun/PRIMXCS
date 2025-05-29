<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.moveFileStatus">PRIMX | 이동파일 현황</title>
<form class="moveFileHistory" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.moveFileStatus"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.status"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.moveFileStatus"></span>
			</h2>
		</div>
		<div class="searchInnerWrap">
			<div class="searchBox">
				<input type="text" class="txtBox searchInput" readonly data-i18n-placeholder="search.searchTit">
			</div>
			<div class="dropMenu">
				<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
				<ul>
					<li class="csv"><button type="button" id="moveFileHistoryCsvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
				</ul>
			</div>
		</div>

	</div>
	<div class="tableWrap">
		<table class="dataTable checkDataTable" id="moveFileHistoryTable">
			<thead>
				<tr>
					<th width="3%" data-i18n="moveFileStatus.grid.number">번호</th>
					<th width="20%" data-i18n="moveFileStatus.grid.fileID">파일 ID</th>
					<th width="5%" data-i18n="moveFileStatus.grid.status">상태</th>
					<th width="15%" data-i18n="moveFileStatus.grid.fileName">파일명</th>
					<th width="8%" data-i18n="moveFileStatus.grid.size">사이즈</th>
					<th width="5%" data-i18n="moveFileStatus.grid.originalFileCount">원본파일수</th>
					<th width="4%" data-i18n="moveFileStatus.grid.remainingCount">잔여횟수</th>
					<th width="10%" data-i18n="moveFileStatus.grid.systemName">시스템명</th>
					<th width="8%" data-i18n="moveFileStatus.grid.user">사용자</th>
					<th width="11%" data-i18n="moveFileStatus.grid.registrationDateTime">등록일시</th>
					<th width="8%" data-i18n="moveFileStatus.grid.endDate">종료일</th>
				</tr>
			</thead>
			<tbody>
				<!-- <tr>
					<td>1</td>
					<td>70A77276-5181-4441-9F8A-BCE167252975</td>
					<td>사용</td>
					<td>WR_4주차주간보고.pptx</td>
					<td>9,200.003 KB</td>
					<td>3</td>
					<td>6</td>
					<td>DESKTOP-6Q8S9V7</td>
					<td>사용자 계정</td>
					<td>2025-01-24 17:28:04</td>
					<td>2025-02-27</td>
				</tr>-->
				<tr>
					<td>2</td>
					<td>371C4BFB-0080-4A94-80CD-169D62CAA5ED</td>
					<td>종료</td>
					<td>(1,+2편)+해설+및+공통보안요구사항.pdf</td>
					<td>9,200.003 KB</td>
					<td>1</td>
					<td>10</td>
					<td>DESKTOP-6Q8S9V7</td>
					<td>사용자 계정</td>
					<td>2025-01-24 17:28:04</td>
					<td>2025-01-21</td>
				</tr> 
			</tbody>
		</table>
	</div>
</form>


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
								<select class="drop" name="dateSelect" id="dateSelect">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="today" data-i18n="search.option.today">오늘</option>
									<option value="this-week" data-i18n="search.option.week">이번주</option>
									<option value="this-month" data-i18n="search.option.month">이번 달</option>
									<option value="custom" data-i18n="search.option.specifyPeriod">기간지정</option>
								</select>
							</div>
							<input type="text" class="datepicker datePickerInner txtBox start-date" name="startDt" id="startDt" data-i18n-placeholder="datepicker.startDate" readonly />
							<input type="text" class="datepicker datePickerInner txtBox end-date" name="endDt" id="endDt" data-i18n-placeholder="datepicker.endDate" readonly />
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.status">상태</label>
							<div class="selectBox">
								<select class="drop" name="status" id="status">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="USE" data-i18n="search.option.use">사용</option>
									<option value="END" data-i18n="search.option.end">종료</option>
								</select>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.condition">조건</label>
							<div class="selectBox">
								<select class="drop" name="condition" id="condition">
									<option value="all" data-i18n="search.option.all">전체</option>
									<option value="fileName" data-i18n="search.option.fileName">파일명</option>
									<option value="computer" data-i18n="search.option.systemName">시스템명</option>
									<option value="user" data-i18n="search.option.userName">사용자명</option>
								</select>
							</div>
							<input type="text" class="txtBox w330" name="searchKeyword" id="searchKeyword" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchMoveFileHistory();}" />
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button data-i18n="button.search" type="button" onclick="searchMoveFileHistory()">검색</button>
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.searchAreaPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 이동파일 현황 상세 보기 -->
<section class="popup moveFileHistoryViewPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="moveFileStatus.moveFileHistoryViewPopup.popTitle">이동파일 정보</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line w2">
						<div class="item">
							<em data-i18n="moveFileStatus.moveFileHistoryViewPopup.registrationDateTime">등록일시</em>
							<div class="" id="insertDate"><!-- 2025-01-24 17:28:04 --></div>
						</div>
						<div class="item">
							<em data-i18n="moveFileStatus.moveFileHistoryViewPopup.status">상태</em>
							<div class="" id="popupStatus"><!-- 사용 --></div>
						</div>
					</li>
					<li class="line w2">
						<div class="item">
							<em data-i18n="moveFileStatus.moveFileHistoryViewPopup.systemName">시스템명</em>
							<div class="" id="computerName"><!-- DESKTOP-6Q8S9V7 --></div>
						</div>
						<div class="item">
							<em data-i18n="moveFileStatus.moveFileHistoryViewPopup.userName">사용자명</em>
							<div class="" id="userNm"><!-- 사용자 계정 --></div>
						</div>
					</li>
				</ul>
				<ul class="setWrap">
					<li class="line">
						<div class="item">
							<em data-i18n="moveFileStatus.moveFileHistoryViewPopup.fileID">파일 ID</em>
							<div id="fileKey"><!-- 70A77276-5181-4441-9F8A-BCE167252975 --></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="moveFileStatus.moveFileHistoryViewPopup.fileName">파일명</em>
							<div id="fileName"><!-- WR_4주차주간보고.pptx --></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="moveFileStatus.moveFileHistoryViewPopup.remainingCount">잔여횟수</em>
							<div id="decryptCnt"><!-- 6 --></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="moveFileStatus.moveFileHistoryViewPopup.endDate">종료일</em>
							<div id="expireDate"><!-- 2025-02-27 --></div>
						</div>
					</li>
				</ul>
				<div class="moveFileTabWrap">
					<ul class="tabTitle">
						<li class="on" data-i18n="moveFileStatus.moveFileHistoryViewPopup.originalFileInfo">원본파일 정보</li>
						<li data-i18n="moveFileStatus.moveFileHistoryViewPopup.moveFileUsageHistory">이동파일 사용 이력</li>
					</ul>
					<ul class="tabCont">
						<li class="on">
							<div class="tableWrap">
								<table class="dataTable normalDataTable" id="originalFileInfoTable">
									<thead>
										<tr>
											<th width="10%" data-i18n="moveFileStatus.moveFileHistoryViewPopup.grid.number">번호</th>
											<th width="65%" data-i18n="moveFileStatus.moveFileHistoryViewPopup.grid.fileName">파일명</th>
											<th width="25%" data-i18n="moveFileStatus.moveFileHistoryViewPopup.grid.size">사이즈</th>
										</tr>
									</thead>
									<tbody>
										<!-- <tr>
											<td>1</td>
											<td>WR_4주차주간보고.pptx</td>
											<td>2.339 KB</td>
										</tr>-->
										<tr>
											<td>2</td>
											<td>스토리보드_User.xlsx</td>
											<td>5.244 KB</td>
										</tr>
										<tr>
											<td>3</td>
											<td>현행업무분석v1.0.docx</td>
											<td>199.07 KB</td>
										</tr> 
									</tbody>
								</table>
							</div>
						</li>
						<li>
							<div class="tableWrap">
								<table class="dataTable normalDataTable" id="moveFileUsageHistoryTable">
									<thead>
										<tr>
											<th width="5%" data-i18n="moveFileStatus.moveFileHistoryViewPopup.grid.number">번호</th>
											<th width="10%" data-i18n="moveFileStatus.moveFileHistoryViewPopup.grid.useDate">사용일자</th>
											<th width="10%" data-i18n="moveFileStatus.moveFileHistoryViewPopup.grid.useTime">사용시각</th>
											<th width="10%" data-i18n="moveFileStatus.moveFileHistoryViewPopup.grid.authRequestIP">인증요청 IP</th>
											<th width="8%" data-i18n="moveFileStatus.moveFileHistoryViewPopup.grid.userName">사용자명</th>
											<th width="10%" data-i18n="moveFileStatus.moveFileHistoryViewPopup.grid.endDate">종료일</th>
											<th width="5%" data-i18n="moveFileStatus.moveFileHistoryViewPopup.grid.remainingCount">잔여횟수</th>
											<th width="5%" data-i18n="moveFileStatus.moveFileHistoryViewPopup.grid.result">결과</th>
											<th width="25%" data-i18n="moveFileStatus.moveFileHistoryViewPopup.grid.rejectionReason">거부사유</th>
										</tr>
									</thead>
									<tbody>
										<!-- <tr>
											<td>1</td>
											<td>2025-02-01</td>
											<td>13:45:54</td>
											<td>192.168.0.131</td>
											<td>2025-02-27</td>
											<td>6</td>
											<td>승인</td>
											<td>-</td>
										</tr>
										<tr>
											<td>2</td>
											<td>2025-02-01</td>
											<td>13:45:28</td>
											<td>192.168.0.131</td>
											<td>2025-02-27</td>
											<td>7</td>
											<td>승인</td>
											<td>-</td>
										</tr>
										<tr>
											<td>3</td>
											<td>2025-02-01</td>
											<td>13:42:40</td>
											<td>192.168.0.131</td>
											<td>2025-02-27</td>
											<td>8</td>
											<td>승인</td>
											<td>-</td>
										</tr>-->
										<tr>
											<td>4</td>
											<td>2025-02-01</td>
											<td>13:40:30</td>
											<td>192.168.0.131</td>
											<td>2025-02-27</td>
											<td>9</td>
											<td>승인</td>
											<td>-</td>
										</tr> 
									</tbody>
								</table>
							</div>
						</li>
					</ul>
				</div>
				<div class="bu_wrap">
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.moveFileHistoryViewPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/file/moveFileHistory.js' />"></script>