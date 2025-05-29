<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/cmm/customTree.js'/>"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/rqt/createRequest.js' />"></script>

<title data-i18n="webTitle.createRequest">PRIMX | 신청</title>
<form action="" class="createRequest">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.createRequest">신청</h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.request">신청 및 결재</span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.createRequest">신청</span>
			</h2>
		</div>
		<div class="searchInnerWrap">
			<div class="searchBox">
				<input type="text" class="txtBox searchInput" onclick="initSearch('searchAreaPopup');" readonly data-i18n-placeholder="search.searchTit">
			</div>
			<div class="dropMenu">
				<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
				<ul>
					<li class="registration registrationBtn disable"><button id="menuRegistrationBtn" class="disabled" type="button" onclick="openCreateRequestPopup();" data-i18n="menu.createRequest" disabled>신청</button></li>
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

<!-- 신청 상세 보기 -->
<section class="popup createRequestViewPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="createRequest.createRequestViewPopup.popTitle">정보</strong>
		<form action="" name="requestViewForm">
			<input type="hidden" name="viewRequestId" />
			<input type="hidden" name="viewRequestCode" />
			<input type="hidden" name="viewRequestApprovalCode" id="viewRequestApprovalCode" />
			<div class="pop_cont">
				<ul class="lineWrap" id="viewApprovers">
					<li class="startLine" data-i18n="createRequest.createRequestPopup.approvalLine">결<br>재<br>선</li>
				</ul>
				<ul class="setWrap">
					<li class="line w2">
						<div class="item">
							<em data-i18n="createRequest.createRequestPopup.requestno">신청 번호</em>
							<div class="documentNumber" id="viewRequestCode"></div>
						</div>
						<div class="item">
							<em data-i18n="createRequest.createRequestPopup.requestDate">신청일자</em>
							<div class="date" id="viewRequestDate"></div>
						</div>
					</li>
					<li class="line w2">
						<div class="item">
							<em data-i18n="createRequest.createRequestPopup.status">상태</em>
							<div class="" id="viewStatusCd"></div>
						</div>
						<div class="item">
							<em data-i18n="createRequest.createRequestPopup.result">결과</em>
							<div class="" id="viewResultCd"></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="createRequest.createRequestPopup.applicationCategory">신청 분류</em>
							<div id="viewRequestApprovalCodeName"></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="createRequest.createRequestPopup.submitter">신청자</em>
							<div id="viewRequestUserNm"></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="createRequest.createRequestPopup.title">제목</em>
							<div><input type="text" class="txtBox" name="viewRequestTitle" id="viewRequestTitle" readonly /></div>
						</div>
					</li>
				</ul>
				<div class="approvalFormTypeM">
					<div class="txtWrap">
						<h3 class="title" data-i18n="createRequest.createRequestPopup.noti">공지</h3>
						<textarea name="" id="createRequestViewNoticeM" readonly></textarea>
					</div>
					<div class="txtWrap">
						<h3 class="title" data-i18n="createRequest.createRequestPopup.registeredFile">등록파일</h3>
						<div class="upFileList" id="viewFiles"></div>
					</div>
					<div class="txtWrap">
						<h3 class="title" data-i18n="createRequest.createRequestPopup.reason">사유</h3>
						<textarea name="viewRequestReasonM" id="viewRequestReasonM" readonly></textarea>
					</div>
				</div>
				<div class="approvalFormTypeC">
					<div class="txtWrap">
						<h3 class="title" data-i18n="createRequest.createRequestPopup.noti">공지</h3>
						<textarea name="" id="createRequestViewNoticeC" readonly></textarea>
					</div>
					<div class="txtWrap">
						<h3 class="title" data-i18n="createRequest.createRequestPopup.accountExtensionPe">계정 연장 기간</h3>
						<input class="datepicker datePickerSetDataEnd txtBox" data-i18n-placeholder="datepicker.endDate" disabled id="usePeriodEnd" />
					</div>
					<div class="txtWrap">
						<h3 class="title" data-i18n="createRequest.createRequestPopup.reason">사유</h3>
						<textarea name="viewRequestReasonC" id="viewRequestReasonC" readonly></textarea>
					</div>
				</div>
				<div class="approvalFormTypeA">
					<div class="txtWrap">
						<h3 class="title" data-i18n="createRequest.createRequestPopup.noti">공지</h3>
						<textarea name="" id="createRequestViewNoticeA" readonly></textarea>
					</div>
					<div class="txtWrap">
						<h3 class="title" data-i18n="createRequest.createRequestPopup.userInfo">등록할 사용자 정보</h3>
						<ul class="setWrap">
							<li class="line">
								<div class="item">
									<em data-i18n="createRequest.createRequestPopup.userId">사용자 ID</em>
									<div class="idCheck">
										<input type="text" class="txtBox" name="userId" placeholder=""  readonly>
										<div class="bu_wrap">
											<button class="disable w160" type="button" onclick="checkUserId($('#userId').val());" data-i18n="button.duplicateCheck" disabled>중복 체크</button>
											<input type="hidden" name="idCheckFlag" value="N">
											<input type="hidden" name="checkedUserId" value="">
										</div>
									</div>
								</div>
							</li>
							<li class="line">
								<div class="item">
									<em data-i18n="createRequest.createRequestPopup.userName">사용자명</em>
									<div class="">
										<input type="text" class="txtBox" name="userName"  readonly>
									</div>
								</div>
							</li>
							<li class="line">
								<div class="item">
									<em data-i18n="createRequest.createRequestPopup.email">이메일</em>
									<div class="idCheck">
										<input type="text" class="txtBox" name="email"  readonly>
										<div class="bu_wrap">
											<button class="disable w160" type="button" onclick="checkMail($('#email').val());" data-i18n="button.duplicateCheck" disabled>중복 체크</button>
											<input type="hidden" name="mailCheckFlag" value="N">
											<input type="hidden" name="checkedMail" value="">
										</div>
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
									<em data-i18n="createRequest.createRequestPopup.department">부서</em>
									<div class="idCheck">
										<input type="text" class="txtBox" name="deptName"  readonly>
										<input type="hidden" name="companyId" id="companyId">
										<input type="hidden" name="deptId" >
										<input type="hidden" name="isDeptIdValid" >
										<div class="bu_wrap">
											<button class="departSelectBtn disable w160" type="button" onclick="openDepartSelectPopup(null, null);" disabled data-i18n="button.selectDepart">부서 선택</button>
										</div>
									</div>
								</div>
							</li>
							<li class="line">
								<div class="item">
									<em data-i18n="createRequest.createRequestPopup.userLevel">사용자 레벨</em>
									<div class="adminLevel">
										<div class="selectBox">
											<select class="drop" name="userRole"  disabled>
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
									<em data-i18n="createRequest.createRequestPopup.usePeriod">사용기간</em>
									<div class="useDate">
										<div class="selectBox">
											<select class="drop" name="usePeriod"  disabled>
												<option value="0" data-i18n="createRequest.createRequestPopup.unlimited">무기한</option>
												<option value="1" data-i18n="createRequest.createRequestPopup.specifyPeriod">기간 지정</option>
											</select>
										</div>
										<input class="datepicker txtBox" name="expiredDate" data-i18n-placeholder="datepicker.endDate" disabled id="expiredDate" />
									</div>
								</div>
							</li>
						</ul>
					</div>
					<div class="txtWrap">
						<h3 class="title" data-i18n="createRequest.createRequestPopup.reason">사유</h3>
						<textarea name="viewRequestReasonA" id="viewRequestReasonA" readonly></textarea>
					</div>
				</div>
				<div class="tableWrap">
					<table class="dataTable normalDataTable" id="apprGrid01">
						<thead>
							<tr>
								<th width="10%" data-i18n="createRequest.createRequestViewPopup.grid.approval">결재</th>
								<th width="15%" data-i18n="createRequest.createRequestViewPopup.grid.approver">결재자</th>
								<th width="15%" data-i18n="createRequest.createRequestViewPopup.grid.department">부서</th>
								<th width="15%" data-i18n="createRequest.createRequestViewPopup.grid.approvalDate">결재일시</th>
								<th width="45%" data-i18n="createRequest.createRequestViewPopup.grid.comment">의견</th>
							</tr>
						</thead>
					</table>
				</div>
				<div class="bu_wrap">
					<button type="button" id="modifiedBtn" onclick="initViewForm('edit');" data-i18n="button.edit">수정</button>
					<button type="button" id="rejectedBtn" onclick="modifiedRequest('PE');" data-i18n="button.retrieval" class="hide">회수</button>
					<button type="button" class="line hide registrationApprovalBtn" id="resultBtn">결재선 등록</button>
					<button type="button" class="save hide" id="saveBtn" onclick="modifiedRequest('PE');" data-i18n="button.save">저장</button>
					<button type="button" onclick="closeViewForm();" data-i18n="button.close" class="lineBtn">닫기</button>
				</div>
			</div>
		</form>

		<button class="bu_close_pop"></button>
	</div>
</section>


<!-- 이동파일 조회 팝업 -->
<section class="popup moveFileListSearchPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="createRequest.moveFileListSearchPopup.popTitle">이동파일 조회</strong>
		<form action="">
			<div class="stickyWrap">
				<div class="searchInnerWrap">
					<div class="searchBox">
						<input type="text" class="txtBox searchInput" onclick="initSearch('moveFileListSearchPopup');" readonly data-i18n-placeholder="search.searchTit">
					</div>
					<div class="dropMenu hide">
						<p class="selectMenu">메뉴</p>
						<ul>
							<li class="registration registrationBtn"><button type="button" onclick="$('.createRequestPopup.innerVerPop').removeClass('hide');">신청</button></li>
							<li class="csv"><button type="button">내보내기(CSV)</button></li>
						</ul>
					</div>
				</div>
			</div>
			<div class="pop_cont">
				<div class="tableWrap">
					<table id="grid02" class="dataTable checkDataTable moveFileListSearchPopupTable">
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
				<div class="bu_wrap">
					<!-- <button class="" type="button" data-i18n="button.confirm">확인</button> -->
					<button class="lineBtn" type="button" onclick="$('.moveFileListSearchPopup').addClass('hide');" data-i18n="button.cancel">취소</button>
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
			<input type="hidden" name="popupName" id="popupName" />
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
					<li class="line" id="approvalCodeLine">
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
							<label for="statusCd" data-i18n="search.searchSubjects.status">상태</label>
							<div class="selectBox">
								<select class="drop" name="statusCd" id="statusCd" data-label="상태">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="CO" data-i18n="search.option.co">완료</option>
									<option value="DR" data-i18n="search.option.pr">진행</option>
									<option value="PE" data-i18n="search.option.pe">대기</option>
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
							<input type="text" class="txtBox w330" name="searchWord" id="searchWord" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchCreateRequest('searchAreaPopup');}" />
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" onclick="searchCreateRequest('searchAreaPopup');" data-i18n="button.search">검색</button>
					<button type="button" class="lineBtn" onclick="$('.searchAreaPopup').addClass('hide');" data-i18n="button.close">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 부서 선택 -->
<section class="popup departSelectPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="departSelectPopup.popTitle">부서 선택</strong>
		<form action="">
			<div class="stickyWrap">
				<div class="searchInnerWrap_p">
					<div class="searchBox">
						<input type="text" class="txtBox searchInput" id="departSelectPopupDeptName" data-i18n-placeholder="search.departmentSearch" onkeypress="if(event.keyCode==13) {event.preventDefault(); openDepartSelectPopup($('#departSelectPopupDeptName').val(), null);}">
						<button type="button" class="absolBtn" onclick="openDepartSelectPopup($('#departSelectPopupDeptName').val(), null);">검색</button>
					</div>
				</div>
			</div>
			<div class="pop_cont">
				<div class="departTreeMap"></div>
				<div class="bu_wrap">
					<button type="button" class="lineBtn" onclick="$('.departSelectPopup').addClass('hide');" data-i18n="button.cancel">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>