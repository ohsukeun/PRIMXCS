<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.approval">PRIMX | 결재</title>

<form action="" class="approvalPage">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.approval"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.request"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.approval"></span>
			</h2>
		</div>
		<div class="searchInnerWrap">
			<div class="searchBox">
				<input type="text" class="txtBox searchInput" readonly data-i18n-placeholder="search.searchTit">
			</div>
			<div class="dropMenu">
				<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
				<ul>
					<!-- 일괄승인 임시주석-->
					<!-- <li class="batchApproval"><button type="button" id="batchApproval" onclick="batchApproval_v1();" data-i18n="menu.batchApproval">일괄 승인</button></li> -->
					<li class="csv"><button type="button" id="csvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
				</ul>
			</div>
		</div>
	</div>
	<div class="tableWrap">
		<table id="grid01" class="dataTable checkDataTable">
			<thead>
				<tr>
					<th width="2%"></th>
					<th width="8%" data-i18n="createRequest.grid.requestno">신청 번호</th>
					<th width="10%" data-i18n="createRequest.grid.applicationCategory">신청 분류</th>
					<th width="7%" data-i18n="createRequest.grid.status">상태</th>
					<th width="7%" data-i18n="createRequest.grid.result">결과</th>
					<th width="7%" data-i18n="createRequest.grid.submitter">신청자</th>
					<th data-i18n="createRequest.grid.title">제목</th>
					<th width="8%" data-i18n="createRequest.grid.createDate">생성일</th>
					<th width="8%" data-i18n="createRequest.grid.requestDate">신청일</th>
					<th width="8%" data-i18n="createRequest.grid.approvalDate">결재일</th>
				</tr>
			</thead>
		</table>
	</div>
</form>

<!-- popup -->
<!-- 결재 상세 보기 -->
<section class="popup createRequestViewPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="approval.createRequestViewPopup.popTitle">정보</strong>
		<form action="" name="requestApprovalViewForm">
			<input type="hidden" name="viewRequestId" />
			<input type="hidden" name="viewRequestCode" />
			<div class="pop_cont">
				<ul class="lineWrap" id="viewApprovers">
					<li class="startLine">결<br>재<br>선</li>
				</ul>
				<ul class="setWrap">
					<li class="line w2">
						<div class="item">
							<em data-i18n="approval.createRequestViewPopup.requestno">신청 번호</em>
							<div class="documentNumber" id="viewRequestCode"></div>
						</div>
						<div class="item">
							<em data-i18n="approval.createRequestViewPopup.requestDate">신청일자</em>
							<div class="date" id="viewRequestDate"></div>
						</div>
					</li>
					<li class="line w2">
						<div class="item">
							<em data-i18n="approval.createRequestViewPopup.status">상태</em>
							<div class="" id="viewStatusCd"></div>
						</div>
						<div class="item">
							<em data-i18n="approval.createRequestViewPopup.result">결과</em>
							<div class="" id="viewResultCd"></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="approval.createRequestViewPopup.applicationCategory">신청 분류</em>
							<div id="viewRequestApprovalCodeName"></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="approval.createRequestViewPopup.submitter">신청자</em>
							<div id="viewRequestUserNm"></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="approval.createRequestViewPopup.title">제목</em>
							<div><input type="text" class="txtBox" name="viewRequestTitle" id="viewRequestTitle" readonly /></div>
						</div>
					</li>
				</ul>
				<div class="approvalFormTypeM">
					<div class="txtWrap">
						<h3 class="title" data-i18n="approval.createRequestViewPopup.noti">공지</h3>
						<textarea name="" id="approvalViewNoticeM" readonly></textarea>
					</div>
					<div class="txtWrap">
						<h3 class="title" data-i18n="approval.createRequestViewPopup.registeredFile">등록파일</h3>
						<div class="upFileList" id="viewFiles"></div>
					</div>
					<div class="txtWrap">
						<h3 class="title" data-i18n="approval.createRequestViewPopup.reason">사유</h3>
						<textarea name="viewRequestReasonM" id="viewRequestReasonM" readonly></textarea>
					</div>
				</div>
				<div class="approvalFormTypeC">
					<div class="txtWrap">
						<h3 class="title" data-i18n="approval.createRequestViewPopup.noti">공지</h3>
						<textarea name="" id="approvalViewNoticeC" readonly></textarea>
					</div>
					<div class="txtWrap">
						<h3 class="title" data-i18n="approval.createRequestViewPopup.accountExtensionPe">계정 연장 기간</h3>
						<input class="datepicker datePickerSetDataEnd txtBox" data-i18n-placeholder="datepicker.endDate" disabled  />
					</div>
					<div class="txtWrap">
						<h3 class="title" data-i18n="approval.createRequestViewPopup.reason">사유</h3>
						<textarea name="viewRequestReasonC" id="viewRequestReasonC" readonly></textarea>
					</div>
				</div>
				<div class="approvalFormTypeA">
					<div class="txtWrap">
						<h3 class="title" data-i18n="approval.createRequestViewPopup.noti">공지</h3>
						<textarea name="" id="approvalViewNoticeA" readonly></textarea>
					</div>
					<div class="txtWrap">
						<h3 class="title" data-i18n="approval.createRequestViewPopup.userInfo">등록할 사용자 정보</h3>
						<ul class="setWrap">
							<li class="line">
								<div class="item">
									<em data-i18n="approval.createRequestViewPopup.userId">사용자 ID</em>
									<div class="idCheck">
										<input type="text" class="txtBox" name="userId" placeholder=""  readonly>
										<div class="bu_wrap">
											<button class="disable" type="button" onclick="checkUserId($('#userId').val());" data-i18n="button.duplicateCheck" disabled>중복 체크</button>
											<input type="hidden" name="idCheckFlag" value="N">
											<input type="hidden" name="checkedUserId" value="">
										</div>
									</div>
								</div>
							</li>
							<li class="line">
								<div class="item">
									<em data-i18n="approval.createRequestViewPopup.userName">사용자명</em>
									<div class="">
										<input type="text" class="txtBox" name="userName"  readonly>
									</div>
								</div>
							</li>
							<li class="line">
								<div class="item">
									<em data-i18n="approval.createRequestViewPopup.email">이메일</em>
									<div class="idCheck">
										<input type="text" class="txtBox" name="email" readonly>
									</div>
								</div>
							</li>
							<!-- <li class="line hide">
								<div class="item">
									<em>회사</em>
									<div class="adminLevel">
										<div class="selectBox">
											<select class="drop" name="companyName"  disabled>
												<option value="ROLE_PRIMX_ADMIN" >회사A</option>
												<option value="ROLE_PRIMX_USER">회사B</option>
											</select>
											<input type="hidden" name="companyId" id="companyId">
										</div>
									</div>
								</div>
							</li> -->
							<li class="line">
								<div class="item">
									<em data-i18n="approval.createRequestViewPopup.department">부서</em>
									<div class="idCheck">
										<input type="text" class="txtBox" name="deptName"  readonly>
										<input type="hidden" name="deptId" >
										<input type="hidden" name="isDeptIdValid" >
										<div class="bu_wrap">
											<button class="departSelectBtn disable" type="button" onclick="openDepartSelectPopup(null, null);" data-i18n="button.selectDepart" disabled>부서 선택</button>
										</div>
									</div>
								</div>
							</li>
							<li class="line">
								<div class="item">
									<em data-i18n="approval.createRequestViewPopup.userLevel">사용자 레벨</em>
									<div class="adminLevel">
										<div class="selectBox">
											<select class="drop" name="userRole" disabled>
												<!-- <option value="ROLE_PRIMX_ADMIN">최고 관리자</option>
												<option value="ROLE_PRIMX_USER">일반 관리자</option>
												<option value="ROLE_PRIMX_USER">사용자</option>
												<option value="ROLE_PRIMX_USER">공용 PC</option> -->
											</select>
										</div>
									</div>
								</div>
							</li>
							<li class="line">
								<div class="item">
									<em data-i18n="approval.createRequestViewPopup.usePeriod">사용기간</em>
									<div class="useDate">
										<div class="selectBox">
											<select class="drop" name="usePeriod"  disabled>
												<option value="0" data-i18n="approval.createRequestViewPopup.unlimited">무기한</option>
												<option value="1" data-i18n="approval.createRequestViewPopup.specifyPeriod">기간 지정</option>
											</select>
										</div>
										<input class="datepicker txtBox" name="expiredDate" data-i18n-placeholder="datepicker.endDate" disabled id="expiredDate" />
									</div>
								</div>
							</li>
						</ul>
					</div>
					<div class="txtWrap">
						<h3 class="title" data-i18n="approval.createRequestViewPopup.reason">사유</h3>
						<textarea name="viewRequestReasonA" id="viewRequestReasonA" readonly></textarea>
					</div>
				</div>
				<div class="tableWrap">
					<table class="dataTable normalDataTable" id="apprGrid01">
						<thead>
							<tr>
								<th width="10%" data-i18n="approval.createRequestViewPopup.grid.approval">결재</th>
								<th width="15%" data-i18n="approval.createRequestViewPopup.grid.approver">결재자</th>
								<th width="15%" data-i18n="approval.createRequestViewPopup.grid.department">부서</th>
								<th width="15%" data-i18n="approval.createRequestViewPopup.grid.approvalDate">결재일시</th>
								<th width="45%" data-i18n="approval.createRequestViewPopup.grid.comment">의견</th>
<!-- 								<th width="4%">order</th> -->
							</tr>
						</thead>
					</table>
				</div>
				<div class="bu_wrap">
					<button type="button" class="delegatedApprovalBtn hide" id="delegatedApprViewBtn" data-i18n="button.delegatedAppHandling">결재처리</button>
					<button type="button" class="approvalBtn hide" id="apprViewBtn" data-i18n="button.appHandling">결재처리</button>
					<button type="button" class="lineBtn" onclick="$('.createRequestViewPopup').addClass('hide');" data-i18n="button.close">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 결재처리 -->
<section class="popup approvalProcessPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="approval.approvalProcessPopup.popTitle">결재처리</strong>
		<form action="" name="requestApprovalForm">
			<input type="hidden" name="delegatedFlag" id="delegatedFlag" />
			<input type="hidden" name="approvalRequestId" id="approvalRequestId" />
			<input type="hidden" name="approvalUserId" id="approvalUserId" />
			<input type="hidden" name="approvalRequestCode" id="approvalRequestCode" />
			<input type="hidden" name="approvalOrderNumber" id="approvalOrderNumber" />
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line">
						<div class="item">
							<em data-i18n="approval.approvalProcessPopup.approver">결재자</em>
							<div class="documentNumber" id="approvalUserNm"></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="approval.approvalProcessPopup.approvalProcess">결재처리</em>
							<div class="appSelect">
								<input class="radio" type="radio" name="approval" id="approvalConfirm" value="AP" checked="checked" />
								<label class="radio_label" for="approvalConfirm" data-i18n="approval.approvalProcessPopup.approval">승인</label>
								<input class="radio" type="radio" name="approval" id="approvalRejected" value="RE" />
								<label class="radio_label" for="approvalRejected" data-i18n="approval.approvalProcessPopup.return">반려</label>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="approval.approvalProcessPopup.approvalComment">결재처리의견</em>
							<div class=""><textarea name="approvalDescription" id="approvalDescription"></textarea></div>
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" class="approvalBtn" onclick="saveApproval()" data-i18n="button.saveApproval">결재 저장</button>
					<button type="button" class="lineBtn" onclick="$('.approvalProcessPopup').addClass('hide');" data-i18n="button.close">닫기</button>
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
		<form action="" name="searchPopupForm" id="searchPopupForm">
			<div class="pop_cont">
				<ul class="search_wrap">
					<li class="line">
						<div class="item">
							<label for="dateSelect" data-i18n="search.searchSubjects.period">기간</label>
							<div class="selectBox">
								<select class="drop" name="dateSelect" id="dateSelect" data-label="기간">
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
							<label for="approvalCode" data-i18n="search.searchSubjects.applicationCategory">신청 분류</label>
							<div class="selectBox">
								<select class="drop" name="approvalCode" id="approvalCode" data-label="상태">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="M" data-i18n="search.option.createMoveFile">이동파일 생성 신청</option>
									<option value="C" data-i18n="search.option.accountExtension">계정 연장 신청</option>
									<option value="A" data-i18n="search.option.userRegistration">사용자 등록 신청</option>
								</select>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label for="resultCd" data-i18n="search.searchSubjects.result">결과</label>
							<div class="selectBox">
								<select class="drop" name="resultCd" id="resultCd" data-label="결과">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="AP" data-i18n="search.option.approval">승인</option>
									<option value="RE" data-i18n="search.option.return">반려</option>
								</select>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label for="searchType" data-i18n="search.searchSubjects.condition">조건</label>
							<div class="selectBox">
								<select class="drop" name="searchType" id="searchType" data-label="조건">
									<option value="all" data-i18n="search.option.all">전체</option>
									<option value="user" data-i18n="search.option.submitter">신청자</option>
									<option value="title" data-i18n="search.option.title">제목</option>
									<option value="dept" data-i18n="search.option.department">부서</option>
								</select>
							</div>
							<input type="text" class="txtBox w330" name="searchWord" id="searchWord" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchApprovalPage('searchAreaPopup');}" />
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" onclick="searchApprovalPage('searchAreaPopup')" data-i18n="button.search">검색</button>
					<button type="button" class="lineBtn" onclick="$('.searchAreaPopup').addClass('hide');" data-i18n="button.close">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/rqt/approval.js' />"></script>