<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.systemStatus">PRIMX | 시스템 현황</title>
<form action="" class="systemStatus">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.systemStatus"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.status"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.systemStatus"></span>
			</h2>
		</div>
		<div class="searchInnerWrap">
			<div class="searchBox">
				<input type="text" class="txtBox searchInput" readonly data-i18n-placeholder="search.searchTit">
			</div>
			<div class="dropMenu">
				<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
				<ul>
					<li class="sysTypeBtn" disabled><button type="button" id="sysTypeBtn" data-i18n="menu.systemType">시스템 종류</button></li>
					<li class="sysDelBtn borbot" disabled><button type="button" id="sysDelBtn" data-i18n="menu.systemDelete">시스템 삭제</button></li>
					<li class="policyChangeBtn" disabled><button type="button" id="policyChangeBtn" data-i18n="menu.productPolicyChange">제품정책 변경</button></li>
					<li class="agentChangeBtn borbot" disabled><button type="button" id="agentChangeBtn" data-i18n="menu.agentPolicyChange">Agent정책 변경</button></li>
					<li class="productHistoryBtn" disabled><button type="button" id="productHistoryBtn" data-i18n="menu.productHistoryStatus">제품이력 현황</button></li>
					<li class="moveFileHistoryBtn borbot" disabled><button type="button" id="moveFileHistoryBtn" data-i18n="menu.moveFileStatus">이동파일 현황</button></li>
					<li class="csv"><button type="button" id="sysStatusExport" data-i18n="menu.csv" class="buttons-csv">내보내기(CSV)</button></li>
				</ul>
			</div>
		</div>
	</div>
	<div class="tableWrap">
		<table id="grid01" class="dataTable checkDataTable">
			<thead>
				<tr>
					<th width="2%"></th>
					<th width="3%" data-i18n="systemStatus.grid.number">번호</th>
					<th data-i18n="systemStatus.grid.systemID">시스템ID</th>
					<th width="12%" data-i18n="systemStatus.grid.systemName">시스템명</th>
					<th width="3%">ZC</th>
					<!-- <th width="3%">OZ</th> -->
					<th width="5%" data-i18n="systemStatus.grid.kind">종류</th>
					<th width="8%" data-i18n="systemStatus.grid.internalExternal">내/외부</th>
					<th width="5%" data-i18n="systemStatus.grid.userName">사용자명</th>
					<th width="7%" data-i18n="systemStatus.grid.departmentName">부서명</th>
					<th width="9%" data-i18n="systemStatus.grid.windowsVersion">Window 버전</th>
					<th width="7%" data-i18n="systemStatus.grid.agentVersion">Agent 버전</th>
					<!-- <th width="7%" data-i18n="systemStatus.grid.policyName">정책명</th>
					<th width="7%" data-i18n="systemStatus.grid.registrationDate">등록일</th>
					<th width="9%" data-i18n="systemStatus.grid.lastLoginDate">최종접속일시</th> -->
				</tr>
			</thead>
		</table>
	</div>
</form>

<!-- popup -->
<!-- 시스템정보 팝업 -->
<section class="popup systemInformationPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="systemStatus.systemInformationPopup.popTitle">시스템 정보</strong>
		<form action="" name="systemInfoForm">
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line w2">
						<div class="item">
							<em data-i18n="systemStatus.systemInformationPopup.registrationDateTime">등록일시</em>
							<div class="" id="viewInsertDate"></div>
						</div>
						<div class="item">
							<em data-i18n="systemStatus.systemInformationPopup.kind">종류</em>
							<div class="" id="viewCommonSystemType"></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="systemStatus.systemInformationPopup.systemID">시스템 ID</em>
							<div class="" id="viewAgentId"></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="systemStatus.systemInformationPopup.systemName">시스템명</em>
							<div class="" id="viewComputerName"></div>
						</div>
					</li>
				</ul>
				<div class="systabWrap">
					<ul class="tabTitle">
						<li class="on" data-i18n="systemStatus.systemInformationPopup.generalInfo">일반 정보</li>
						<li data-i18n="systemStatus.systemInformationPopup.osInfo">OS 정보</li>
						<li data-i18n="systemStatus.systemInformationPopup.diskInfo">디스크 정보</li>
						<li data-i18n="systemStatus.systemInformationPopup.nicInfo">NIC 정보</li>
						<li data-i18n="systemStatus.systemInformationPopup.productInfo">제품 정보</li>
					</ul>
					<ul class="tabCont">
						<li class="on">
							<div class="tableWrap leftTable">
								<table>
									<colgroup>
										<col width="20%" />
										<col width="80%" />
									</colgroup>
									<tr>
										<th scope="row" data-i18n="systemStatus.systemInformationPopup.workGroup">작업 그룹</th>
										<td id="viewWorkgroupName"></td>
									</tr>
									<tr>
										<th scope="row" data-i18n="systemStatus.systemInformationPopup.usedDomain">사용 도메인</th>
										<td id="viewDomainName"></td>
									</tr>
									<tr>
										<th scope="row" data-i18n="systemStatus.systemInformationPopup.agentVersion">Agent 버전</th>
										<td id="viewAgentVersion"></td>
									</tr>
									<tr>
										<th scope="row" data-i18n="systemStatus.systemInformationPopup.lastAgentPolicyName">최종 Agent정책명</th>
										<td id="viewAgentPolicyName"></td>
									</tr>
									<tr>
										<th scope="row" data-i18n="systemStatus.systemInformationPopup.user">사용자</th>
										<td id="viewUserNm"></td>
									</tr>
									<tr>
										<th scope="row" data-i18n="systemStatus.systemInformationPopup.lastLoginDate">최종 접속 일시</th>
										<td id="viewLastKaDate"></td>
									</tr>
								</table>
							</div>
						</li>
						<li>
							<div class="tableWrap leftTable">
								<table>
									<colgroup>
										<col width="20%" />
										<col width="80%" />
									</colgroup>
									<tr>
										<th scope="row" data-i18n="systemStatus.systemInformationPopup.windowsVersion">Windows버전</th>
										<td id="viewWinVersionString"></td>
									</tr>
									<tr>
										<th scope="row">Windows UUID</th>
										<td id="viewWinUuid"></td>
									</tr>
									<tr>
										<th scope="row" data-i18n="systemStatus.systemInformationPopup.osMajorVersion">OS 주 버전</th>
										<td id="viewOsMajorVersion"></td>
									</tr>
									<tr>
										<th scope="row" data-i18n="systemStatus.systemInformationPopup.osBuildNumber">OS 빌드번호</th>
										<td id="viewOsBuildNumber"></td>
									</tr>
									<tr>
										<th scope="row" data-i18n="systemStatus.systemInformationPopup.osServicePackMajorVersion">OS 서비스팩 주 버전</th>
										<td id="viewOsServicepackMajor"></td>
									</tr>
									<tr>
										<th scope="row" data-i18n="systemStatus.systemInformationPopup.osServicePackMinorVersion">OS 서비스팩 부 버전</th>
										<td id="viewOsServicepackMinor"></td>
									</tr>
								</table>
							</div>
						</li>
						<li>
							<div class="tableWrap leftTable doubleTable">
								<table id="diskInfoTable">
									<colgroup>
										<col width="15%" />
										<col width="20%" />
										<col width="65%" />
									</colgroup>
									<tbody></tbody>
								</table>
							</div>
						</li>
						<li>
							<div class="tableWrap leftTable doubleTable">
								<table id="nicInfoTable">
									<colgroup>
										<col width="15%" />
										<col width="20%" />
										<col width="65%" />
									</colgroup>
									<tbody></tbody>
								</table>
							</div>
						</li>
						<li>
							<div class="tableWrap leftTable doubleTable">
								<table id="setupInfoTable">
									<colgroup>
										<col width="15%" />
										<col width="20%" />
										<col width="65%" />
									</colgroup>
									<tbody></tbody>
								</table>
							</div>
						</li>
					</ul>
				</div>
				<div class="bu_wrap">
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.systemInformationPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button type="button" class="bu_close_pop"></button>
	</div>
</section>

<!-- 시스템 종류 변경 -->
<section class="popup changeSystemTypePopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="systemStatus.changeSystemTypePopup.popTitle">시스템 종류 변경</strong>
		<form action="" name="changeSystemTypeForm">
			<input type="hidden" name="changeSystemId" id="changeSystemId"/>
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line">
						<div class="item">
							<em>
								<input type="radio" name="sysTypeSelectRadio" id="sysTypeSelectRadio_Y" value="Y">
								<label for="sysTypeSelectRadio_Y" data-i18n="systemStatus.changeSystemTypePopup.publicSystem">공용 시스템</label>
							</em>
							<div class="" data-i18n="systemStatus.changeSystemTypePopup.useAsSystemWithoutUser">사용자 없는 시스템으로 사용</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em>
								<input type="radio" name="sysTypeSelectRadio" id="sysTypeSelectRadio_N" value="N">
								<label for="sysTypeSelectRadio_N" data-i18n="systemStatus.changeSystemTypePopup.userSystem">사용자 시스템</label>
							</em>
							<div class="" data-i18n="systemStatus.changeSystemTypePopup.useGuestAccountUntilUserRegistration">사용자 등록전 까지 게스트 사용자 계정 사용</div>
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" onclick="fnChangeSystemType();" data-i18n="button.change">변경</button>
					<button type="button" onclick="$('.changeSystemTypePopup').addClass('hide');" class="lineBtn" data-i18n="button.cancel">취소</button>
				</div>
			</div>
		</form>
		<button type="button" class="bu_close_pop"></button>
	</div>
</section>

<!-- 시스템 삭제 -->
<section class="popup sysDelPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="systemStatus.sysDelPopup.popTitle">시스템 삭제</strong>
		<form action="">
			<div class="pop_cont">
				<span data-i18n="systemStatus.sysDelPopup.delTxt">시스템을 삭제 하시겠습니까?</span>
				<div class="bu_wrap">
					<button type="button" data-i18n="button.confirm" onclick="fnSysDel();">확인</button>
					<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="fnSysDelCancel();">취소</button>
				</div>
			</div>
		</form>
		<button type="button" class="bu_close_pop"></button>
	</div>
</section>

<!-- 제품 정책 변경 -->
<section class="popup policyChangePopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="systemStatus.policyChangePopup.popTitle">제품 정책 변경</strong>
		<form action="">
			<div class="stickyWrap">
				<div class="selectBox mb15 wAuto">
					<select class="drop" name="">
						<option value="1" data-i18n="systemStatus.policyChangePopup.zon">제품 : ZONECENTRAL</option>
						<!-- <option value="2" data-i18n="systemStatus.policyChangePopup.ori">제품 : ORIZON</option> -->
					</select>
				</div>
				<div class="searchInnerWrap_onlySc">
					<div class="searchBox">
						<input type="text" class="txtBox searchInput" name="searchKeyword" data-i18n-placeholder="systemStatus.policyChangePopup.policyNameSearch" onkeypress="if(event.keyCode==13) {event.preventDefault(); loadProductPolicyTable();}">
						<button type="button" class="absolBtn" onclick="loadProductPolicyTable()">검색</button>
					</div>
				</div>
			</div>
			<div class="pop_cont">
				<div class="zonecentralArea">
					<div class="tableWrap">
						<table class="dataTable onlyCheckDataTable" id="zonecentralPolicyTable">
							<thead>
								<tr>
									<th width="3%" data-i18n="systemStatus.policyChangePopup.grid.select">선택</th>
									<th width="3%" data-i18n="systemStatus.policyChangePopup.grid.number">번호</th>
									<th width="17%" data-i18n="systemStatus.policyChangePopup.grid.policyID">정책 ID</th>
									<th width="10%" data-i18n="systemStatus.policyChangePopup.grid.product">제품</th>
									<th width="10%" data-i18n="systemStatus.policyChangePopup.grid.policyName">정책명</th>
									<th width="5%" data-i18n="systemStatus.policyChangePopup.grid.type">유형</th>
									<th width="3%" data-i18n="systemStatus.policyChangePopup.grid.apply">적용</th>
									<th width="10%" data-i18n="systemStatus.policyChangePopup.grid.registrant">등록자</th>
									<th width="10%" data-i18n="systemStatus.policyChangePopup.grid.registrationDate">등록일</th>
									<th width="10%" data-i18n="systemStatus.policyChangePopup.grid.modificationDate">수정일</th>
								</tr>
							</thead>
							<tbody>
								<!-- <tr>
									<td></td>
									<td>1</td>
									<td>POL-2024110800001</td>
									<td>ZONECENTRAL</td>
									<td>ZC_default</td>
									<td class="result"><span class="defaultPol">기본</span></td>
									<td>1</td>
									<td>admin 관리자</td>
									<td>2024-08-21</td>
									<td>2024-08-29</td>
								</tr>
								<tr>
									<td></td>
									<td>2</td>
									<td>POL-2024110800002</td>
									<td>ZONECENTRAL</td>
									<td>ZC_sub001</td>
									<td class="result"><span class="eachPol">개별</span></td>
									<td>5</td>
									<td>admin 관리자</td>
									<td>2024-08-22</td>
									<td>2024-08-30</td>
								</tr> -->
							</tbody>
						</table>
					</div>
				</div>
				<div class="orizonArea hide">
					<div class="tableWrap">
						<table class="dataTable onlyCheckDataTable" id="orizonPolicyTable">
							<thead>
								<tr>
									<th width="3%" data-i18n="systemStatus.policyChangePopup.grid.number">번호</th>
									<th width="20%" data-i18n="systemStatus.policyChangePopup.grid.policyID">정책 ID</th>
									<th width="10%" data-i18n="systemStatus.policyChangePopup.grid.product">제품</th>
									<th width="10%" data-i18n="systemStatus.policyChangePopup.grid.policyName">정책명</th>
									<th width="5%" data-i18n="systemStatus.policyChangePopup.grid.type">유형</th>
									<th width="3%" data-i18n="systemStatus.policyChangePopup.grid.apply">적용</th>
									<th width="10%" data-i18n="systemStatus.policyChangePopup.grid.registrant">등록자</th>
									<th width="10%" data-i18n="systemStatus.policyChangePopup.grid.registrationDate">등록일</th>
									<th width="10%" data-i18n="systemStatus.policyChangePopup.grid.modificationDate">수정일</th>
								</tr>
							</thead>
							<tbody>
								<!-- <tr>
									<td></td>
									<td>1</td>
									<td>POL-2024110800011</td>
									<td>ORIZON</td>
									<td>OZ_default</td>
									<td class="result"><span class="defaultPol">기본</span></td>
									<td>1</td>
									<td>admin 관리자</td>
									<td>2024-08-21</td>
									<td>2024-08-29</td>
								</tr>
								<tr>
									<td>2</td>
									<td>POL-2024110800012</td>
									<td>ORIZON</td>
									<td>OZ_sub001</td>
									<td class="result"><span class="eachPol">개별</span></td>
									<td>5</td>
									<td>admin 관리자</td>
									<td>2024-08-22</td>
									<td>2024-08-30</td>
								</tr> -->
							</tbody>
						</table>
					</div>
				</div>
				<div class="bu_wrap">
					<button type="button" data-i18n="button.apply" onclick="changeProductPolicy()">적용</button>
					<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="$('.policyChangePopup').addClass('hide');">취소</button>
				</div>
			</div>
		</form>
		<button type="button" class="bu_close_pop"></button>
	</div>
</section>

<!-- Agent 정책 변경 -->
<section class="popup agentChangePopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="systemStatus.agentChangePopup.popTitle">Agent정책 변경</strong>
		<form action="">
			<div class="stickyWrap">
				<div class="searchInnerWrap_onlySc">
					<div class="searchBox">
						<input type="text" class="txtBox searchInput" name="searchKeyword" data-i18n-placeholder="systemStatus.agentChangePopup.policyNameSearch" onkeypress="if(event.keyCode==13) {event.preventDefault(); loadAgentPolicyTable();}">
						<button type="button" class="absolBtn" onclick="loadAgentPolicyTable()">검색</button>
					</div>
				</div>
			</div>
			<div class="pop_cont">
				<div class="agentArea">
					<div class="tableWrap">
						<table class="dataTable onlyCheckDataTable" id="agentPolicyTable">
							<thead>
								<tr>
									<th width="3%" data-i18n="systemStatus.agentChangePopup.grid.select">선택</th>
									<th width="3%" data-i18n="systemStatus.agentChangePopup.grid.number">번호</th>
									<th width="20%" data-i18n="systemStatus.agentChangePopup.grid.policyID">정책 ID</th>
									<th width="10%" data-i18n="systemStatus.agentChangePopup.grid.policyName">정책명</th>
									<th width="5%" data-i18n="systemStatus.agentChangePopup.grid.type">유형</th>
									<th width="3%" data-i18n="systemStatus.agentChangePopup.grid.apply">적용</th>
									<th width="10%" data-i18n="systemStatus.agentChangePopup.grid.registrant">등록자</th>
									<th width="10%" data-i18n="systemStatus.agentChangePopup.grid.registrationDate">등록일</th>
									<th width="10%" data-i18n="systemStatus.agentChangePopup.grid.modificationDate">수정일</th>
								</tr>
							</thead>
							<tbody>
								<!-- <tr>
									<td></td>
									<td>1</td>
									<td>AGN-2024110800011</td>
									<td>기본 정책</td>
									<td class="result"><span class="defaultPol">기본</span></td>
									<td>1</td>
									<td>admin 관리자</td>
									<td>2024-08-21</td>
									<td>2024-08-29</td>
								</tr>
								<tr>
									<td></td>
									<td>2</td>
									<td>AGN-2024110800012</td>
									<td>개별 정책</td>
									<td class="result"><span class="eachPol">개별</span></td>
									<td>5</td>
									<td>admin 관리자</td>
									<td>2024-08-22</td>
									<td>2024-08-30</td>
								</tr> -->
							</tbody>
						</table>
					</div>
				</div>
				<div class="bu_wrap">
					<button type="button" data-i18n="button.apply" onclick="addAgentSystem()">적용</button>
					<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="$('.agentChangePopup').addClass('hide');">취소</button>
				</div>
			</div>
		</form>
		<button type="button" class="bu_close_pop"></button>
	</div>
</section>

<!-- 제품이력 현황 -->
<section class="popup productHistoryPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="systemStatus.productHistoryPopup.popTitle">제품이력 현황</strong>
		<form action="">
			<div class="stickyWrap">
				<div class="searchInnerWrap_p">
					<div class="searchBox">
						<input type="text" class="txtBox searchInput" readonly data-i18n-placeholder="search.searchTit" name="searchKeyword" value="${searchVO.searchKeyword}" onclick="openProductHistorySearchPopup();">
						<!-- <p class="filterItem">
							<span class="filterTitle">상태</span>
							<span> : </span>
							<span class="filterSelect">완료</span>
							<span class="delBtn"></span>
						</p> -->
					</div>
					<div class="dropMenu">
						<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
						<ul>
							<li class="csv"><button type="button" id="productHistoryPopupCsvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
						</ul>
					</div>
				</div>
			</div>
			<div class="pop_cont">
				<div class="tableWrap">
					<table class="dataTable checkDataTable" id="productHistoryDataTable">
						<thead>
							<tr>
								<th width="3%"></th>
								<th width="10%" data-i18n="systemStatus.productHistoryPopup.grid.date">일자</th>
								<th width="10%" data-i18n="systemStatus.productHistoryPopup.grid.time">시각</th>
								<th data-i18n="systemStatus.productHistoryPopup.grid.product">제품</th>
								<th width="5%" data-i18n="systemStatus.productHistoryPopup.grid.level">레벨</th>
								<th width="10%" data-i18n="systemStatus.productHistoryPopup.grid.kind">종류</th>
								<th width="20%" data-i18n="systemStatus.productHistoryPopup.grid.content">내용</th>
								<th width="13%" data-i18n="systemStatus.productHistoryPopup.grid.systemName">시스템명</th>
								<th width="12%" data-i18n="systemStatus.productHistoryPopup.grid.deptName">부서명</th>
								<th width="8%" data-i18n="systemStatus.productHistoryPopup.grid.userName">사용자명</th>
							</tr>
						</thead>
						<tbody>
							<!-- <tr>
								<td></td>
								<td>1</td>
								<td>ZONECENTRAL</td>
								<td>INFO</td>
								<td>시스템 동작</td>
								<td>ONE 서비스가 시작 되었습니다.</td>
								<td>DESKTOP-6Q8S9V7</td>
								<td>김소정</td>
								<td>김소정</td>
								<td>2024-08-21</td>
								<td>17:43:09</td>
							</tr> -->
						</tbody>
					</table>
				</div>
				<div class="bu_wrap">
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.productHistoryPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button type="button" class="bu_close_pop"></button>
	</div>
</section>
<!-- 제품이력 현황 > 검색 팝업 -->
<section class="popup productHistorySearchPop searchArea_p_Popup hide">
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
								<select class="drop" name="dateSelectPopProduct" id="dateSelectPopProduct" data-label="기간">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="today" data-i18n="search.option.today">오늘</option>
									<option value="this-week" data-i18n="search.option.week">이번주</option>
									<option value="this-month" data-i18n="search.option.month">이번 달</option>
									<option value="custom" data-i18n="search.option.specifyPeriod">기간지정</option>
								</select>
							</div>
							<input type="text" class="datepicker datePickerPopProduct txtBox start-date" name="startDt" id="startDt2" data-i18n-placeholder="datepicker.startDate" readonly />
							<input type="text" class="datepicker datePickerPopProduct txtBox end-date" name="endDt" id="endDt2" data-i18n-placeholder="datepicker.endDate" readonly />
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.level">레벨</label>
							<div class="selectBox">
								<select class="drop" name="searchLevel" >
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
								<select class="drop" name="searchProduct" >
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
								<select class="drop" name="searchType" >
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
								<select class="drop" name="condition" >
									<option value="all" data-i18n="search.option.all">전체</option>
									<option value="computerName" data-i18n="search.option.systemName">시스템명</option>
									<option value="userName" data-i18n="search.option.userName">사용자명</option>
									<option value="deptName" data-i18n="search.option.departmentName">부서명</option>
								</select>
							</div>
							<input type="text" class="txtBox w330" name="searchKeyword" id="searchKeyword" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchSystemStatus('productHistorySearchPop');}" />
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" onclick="searchSystemStatus('productHistorySearchPop')" data-i18n="button.search">검색</button>
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.productHistorySearchPop').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button type="button" class="bu_close_pop"></button>
	</div>
</section>
<!-- 제품이력 현황 상세 보기 -->
<section class="popup productHistoryViewPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="systemStatus.productHistoryViewPopup.popTitle">정보</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line w2">
						<div class="item">
							<em data-i18n="systemStatus.productHistoryViewPopup.dateTime">일시</em>
							<div class=""></div>
						</div>
						<div class="item">
							<em data-i18n="systemStatus.productHistoryViewPopup.level">레벨</em>
							<div class=""></div>
						</div>
					</li>
					<li class="line w2">
						<div class="item">
							<em data-i18n="systemStatus.productHistoryViewPopup.product">제품</em>
							<div class=""></div>
						</div>
						<div class="item">
							<em data-i18n="systemStatus.productHistoryViewPopup.kind">종류</em>
							<div class=""></div>
						</div>
					</li>
				</ul>
				<ul class="setWrap setWrapScroll mb00">
					<li class="line">
						<div class="item">
							<em data-i18n="systemStatus.productHistoryViewPopup.systemName">시스템명</em>
							<div></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="systemStatus.productHistoryViewPopup.userName">사용자명</em>
							<div></div>
						</div>
					</li>
					<!-- <li class="line">
						<div class="item">
							<em data-i18n="systemStatus.productHistoryViewPopup.workerName">작업자명</em>
							<div></div>
						</div>
					</li> -->
					<li class="line">
						<div class="item">
							<em data-i18n="systemStatus.productHistoryViewPopup.content">내용</em>
							<div class="contentArea"></div>
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.productHistoryViewPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button type="button" class="bu_close_pop"></button>
	</div>
</section>

<!-- 이동파일 현황 -->
<section class="popup moveFileHistoryPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="systemStatus.moveFileHistoryPopup.popTitle">이동파일 현황</strong>
		<form action="">
			<div class="stickyWrap">
				<div class="searchInnerWrap_p">
					<div class="searchBox">
						<input type="text" class="txtBox searchInput" readonly data-i18n-placeholder="search.searchTit" onclick="openMoveFileHistorySearchPopup();">
						<!-- <p class="filterItem">
							<span class="filterTitle">상태</span>
							<span> : </span>
							<span class="filterSelect">완료</span>
							<span class="delBtn"></span>
						</p> -->
					</div>
					<div class="dropMenu">
						<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
						<ul>
							<li class="csv"><button type="button" id="moveFileHistoryCsvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
						</ul>
					</div>
				</div>
			</div>
			<div class="pop_cont">
				<div class="moveFileHistoryArea">
					<div class="tableWrap">
						<table class="dataTable checkDataTable" id="moveFileHistoryTable">
							<thead>
							<tr>
								<th width="3%"></th>
								<th width="3%" data-i18n="systemStatus.moveFileHistoryPopup.grid.number">번호</th>
								<th width="20%" data-i18n="systemStatus.moveFileHistoryPopup.grid.fileID">파일 ID</th>
								<th width="5%" data-i18n="systemStatus.moveFileHistoryPopup.grid.status">상태</th>
								<th width="15%" data-i18n="systemStatus.moveFileHistoryPopup.grid.fileName">파일명</th>
								<th width="8%" data-i18n="systemStatus.moveFileHistoryPopup.grid.size">사이즈</th>
								<th width="5%" data-i18n="systemStatus.moveFileHistoryPopup.grid.originalFileCount">원본파일수</th>
								<th width="4%" data-i18n="systemStatus.moveFileHistoryPopup.grid.remainingCount">잔여횟수</th>
								<th width="10%" data-i18n="systemStatus.moveFileHistoryPopup.grid.systemName">시스템명</th>
								<th width="8%" data-i18n="systemStatus.moveFileHistoryPopup.grid.user">사용자</th>
								<th width="11%" data-i18n="systemStatus.moveFileHistoryPopup.grid.registrationDateTime">등록일시</th>
								<th width="8%" data-i18n="systemStatus.moveFileHistoryPopup.grid.endDate">종료일</th>
							</tr>
						</thead>
						<tbody>
							<!-- <tr>
								<td></td>
								<td>1</td>
								<td>70A77276-5181-4441-9F8A-BCE167252975</td>
								<td>종료</td>
								<td>WR_4주차주간보고.pptx</td>
								<td>9,200.003 KB</td>
								<td>3</td>
								<td>7</td>
								<td>DESKTOP-6Q8S9V7</td>
								<td>김소정</td>
								<td>2024-08-21 17:43:09</td>
								<td>2024-09-21</td>
							</tr> -->
						</tbody>
						</table>
					</div>
				</div>
				<div class="bu_wrap">
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.moveFileHistoryPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button type="button" class="bu_close_pop"></button>
	</div>
</section>
<!-- 이동파일 현황 > 검색 팝업 -->
<section class="popup moveFileHistorySearchPop searchArea_p_Popup hide">
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
								<select class="drop" name="dateSelectPopMfile" id="dateSelectPopMfile" data-label="기간">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="today" data-i18n="search.option.today">오늘</option>
									<option value="this-week" data-i18n="search.option.week">이번주</option>
									<option value="this-month" data-i18n="search.option.month">이번 달</option>
									<option value="custom" data-i18n="search.option.specifyPeriod">기간지정</option>
								</select>
							</div>
							<input type="text" class="datepicker datePickerPopMfile txtBox start-date" name="startDt" id="startDt3" data-i18n-placeholder="datepicker.startDate" readonly />
							<input type="text" class="datepicker datePickerPopMfile txtBox end-date" name="endDt" id="endDt3" data-i18n-placeholder="datepicker.endDate" readonly />
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.status">상태</label>
							<div class="selectBox">
								<select class="drop" name="status" >
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
								<select class="drop" name="condition" >
									<option value="all" data-i18n="search.option.all">전체</option>
									<option value="fileName" data-i18n="search.option.fileName">파일명</option>
									<option value="computer" data-i18n="search.option.systemName">시스템명</option>
									<option value="user" data-i18n="search.option.userName">사용자명</option>
								</select>
							</div>
							<input type="text" class="txtBox w330" name="searchKeyword" id="searchKeyword" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchSystemStatus('moveFileHistorySearchPop');}" />
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" data-i18n="button.search" onclick="searchSystemStatus('moveFileHistorySearchPop')">검색</button>
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.moveFileHistorySearchPop').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button type="button" class="bu_close_pop"></button>
	</div>
</section>
<!-- 이동파일 현황 상세 보기 -->
<section class="popup moveFileHistoryViewPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="systemStatus.moveFileHistoryViewPopup.popTitle">이동파일 정보</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line w2">
						<div class="item">
							<em data-i18n="systemStatus.moveFileHistoryViewPopup.registrationDateTime">등록일시</em>
							<div class="" name="insertDate" ><!-- 2024-08-21 17:43:09 --></div>
						</div>
						<div class="item">
							<em data-i18n="systemStatus.moveFileHistoryViewPopup.status">상태</em>
							<div class="" name="status" ><!-- 종료 --></div>
						</div>
					</li>
					<li class="line w2">
						<div class="item">
							<em data-i18n="systemStatus.moveFileHistoryViewPopup.systemName">시스템명</em>
							<div class="" name="computerName" ><!-- DESKTOP-6Q8S9V7 --></div>
						</div>
						<div class="item">
							<em data-i18n="systemStatus.moveFileHistoryViewPopup.userName">사용자명</em>
							<div class="" name="userNm" ><!-- 김소정 --></div>
						</div>
					</li>
				</ul>
				<ul class="setWrap">
					<li class="line">
						<div class="item">
							<em data-i18n="systemStatus.moveFileHistoryViewPopup.fileID">파일 ID</em>
							<div class="" name="fileKey" ><!-- 70A77276-5181-4441-9F8A-BCE167252975 --></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="systemStatus.moveFileHistoryViewPopup.fileName">파일명</em>
							<div class="" name="fileName" ><!-- WR_4주차주간보고.pptx --></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="systemStatus.moveFileHistoryViewPopup.remainingCount">잔여횟수</em>
							<div class="" name="decryptCnt" ><!-- 6 --></div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="systemStatus.moveFileHistoryViewPopup.endDate">종료일</em>
							<div class="" name="expireDate" ><!-- 2024-09-21 --></div>
						</div>
					</li>
				</ul>
				<div class="moveFileTabWrap">
					<ul class="tabTitle">
						<li class="on" data-i18n="systemStatus.moveFileHistoryViewPopup.originalFileInfo">원본파일 정보</li>
						<li data-i18n="systemStatus.moveFileHistoryViewPopup.moveFileUsageHistory">이동파일 사용 이력</li>
					</ul>
					<ul class="tabCont">
						<li class="on">
							<div class="tableWrap">
								<table class="dataTable normalDataTable" id="originalFileInfoTable">
									<thead>
										<tr>
											<th width="10%" data-i18n="systemStatus.moveFileHistoryViewPopup.grid.number">번호</th>
											<th width="65%" data-i18n="systemStatus.moveFileHistoryViewPopup.grid.fileName">파일명</th>
											<th width="25%" data-i18n="systemStatus.moveFileHistoryViewPopup.grid.size">사이즈</th>
										</tr>
									</thead>
									<tbody>
										<!-- <tr>
											<td>1</td>
											<td>WR_4주차주간보고.pptx</td>
											<td>2.339 KB</td>
										</tr>
										<tr>
											<td>2</td>
											<td>스토리보드_User.xlsx</td>
											<td>5.244 KB</td>
										</tr>
										<tr>
											<td>3</td>
											<td>현행업무분석v1.0.docx</td>
											<td>199.07 KB</td>
										</tr> -->
									</tbody>
								</table>
							</div>
						</li>
						<li>
							<div class="tableWrap">
								<table class="dataTable normalDataTable" id="moveFileUsageHistoryTable">
									<thead>
										<tr>
											<th width="5%" data-i18n="systemStatus.moveFileHistoryViewPopup.grid.number">번호</th>
											<th width="10%" data-i18n="systemStatus.moveFileHistoryViewPopup.grid.useDate">사용일자</th>
											<th width="10%" data-i18n="systemStatus.moveFileHistoryViewPopup.grid.useTime">사용시각</th>
											<th width="10%" data-i18n="systemStatus.moveFileHistoryViewPopup.grid.authRequestIP">인증요청 IP</th>
											<th width="8%" data-i18n="systemStatus.moveFileHistoryViewPopup.grid.userName">사용자명</th>
											<th width="10%" data-i18n="systemStatus.moveFileHistoryViewPopup.grid.endDate">종료일</th>
											<th width="5%" data-i18n="systemStatus.moveFileHistoryViewPopup.grid.remainingCount">잔여횟수</th>
											<th width="5%" data-i18n="systemStatus.moveFileHistoryViewPopup.grid.result">결과</th>
											<th width="25%" data-i18n="systemStatus.moveFileHistoryViewPopup.grid.rejectionReason">거부사유</th>
										</tr>
									</thead>
									<tbody>
										<!-- <tr>
											<td>1</td>
											<td>2024-10-31</td>
											<td>13:45:54</td>
											<td>192.168.0.131</td>
											<td>2024-11-03</td>
											<td>6</td>
											<td>승인</td>
											<td>-</td>
										</tr>
										<tr>
											<td>2</td>
											<td>2024-10-31</td>
											<td>13:45:28</td>
											<td>192.168.0.131</td>
											<td>2024-11-03</td>
											<td>7</td>
											<td>승인</td>
											<td>-</td>
										</tr>
										<tr>
											<td>3</td>
											<td>2024-10-31</td>
											<td>13:42:40</td>
											<td>192.168.0.131</td>
											<td>2024-11-03</td>
											<td>8</td>
											<td>승인</td>
											<td>-</td>
										</tr>
										<tr>
											<td>4</td>
											<td>2024-10-31</td>
											<td>13:40:30</td>
											<td>192.168.0.131</td>
											<td>2024-11-03</td>
											<td>9</td>
											<td>승인</td>
											<td>-</td>
										</tr> -->
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
		<button type="button" class="bu_close_pop"></button>
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
							<label for="lastKaDate" data-i18n="search.searchSubjects.inactivePeriod">미접속 기간</label>
							<div class="selectBox">
								<select class="drop" name="lastKaDateType" id="lastKaDateType" data-label="미접속 기간">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="7D" data-i18n="search.option.moreThan7Days">7일 이상</option>
									<option value="1M" data-i18n="search.option.moreThan1Month">1개월 이상</option>
									<option value="3M" data-i18n="search.option.moreThan3Months">3개월 이상</option>
									<option value="6M" data-i18n="search.option.moreThan6Months">6개월 이상</option>
								</select>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label for="commonSystemYn" data-i18n="search.searchSubjects.systemType">시스템 종류</label>
							<div class="selectBox">
								<select class="drop" name="commonSystemYn" id="commonSystemYn" data-label="시스템 종류">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="Y" data-i18n="search.option.publicSystem">공용 시스템</option>
									<option value="N" data-i18n="search.option.userSystem">사용자 시스템</option>
								</select>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label for="productId" data-i18n="search.searchSubjects.installedProduct">설치 제품</label>
							<div class="selectBox">
								<select class="drop" name="productName" id="productName" data-label="설치 제품">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="1">ZONECENTRAL</option>
									<!-- <option value="2">ORIZON</option> -->
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
									<option value="computer" data-i18n="search.option.systemName">시스템명</option>
									<option value="user" data-i18n="search.option.userName">사용자명</option>
									<option value="dept" data-i18n="search.option.departmentName">부서명</option>
								</select>
							</div>
							<input type="text" class="txtBox w330" name="searchKeyword" id="searchWord" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchSystemStatus('searchAreaPopup');}" />
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" onclick="searchSystemStatus('searchAreaPopup')" data-i18n="button.search">검색</button>
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.searchAreaPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button type="button" class="bu_close_pop"></button>
	</div>
</section>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/sys/systemStatus.js' />"></script>