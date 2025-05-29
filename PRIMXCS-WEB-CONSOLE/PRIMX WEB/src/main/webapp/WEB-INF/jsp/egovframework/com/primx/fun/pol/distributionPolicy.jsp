<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.deploymentPolicy">PRIMX | 배포 정책</title>
	<form class="distributionPolicy" action="">
		<div class="stickyWrap">
			<div class="titleWrap">
				<h2 class="title" data-i18n="navigation.deploymentPolicy"></h2>
				<h2 class="depthTitle">
					<span class="fir" data-i18n="navigation.policy"></span>
					<span class="fir"> > </span>
					<span class="sec" data-i18n="navigation.deploymentPolicy"></span>
				</h2>
			</div>
		</div>
		<div class="departUserSection">
			<div class="departmentArea">
				<div class="departTreeMap" id="deptTreeContainer">
					<!-- 트리가 여기에 렌더링됨 -->
				</div>
			</div>
			<div class="userArea">
				<div class="userBottom">
					<div class="searchInnerWrap_p mb15">
						<div class="searchBox">
							<input type="text" class="txtBox searchInput2" readonly data-i18n-placeholder="search.searchTit" name="searchKeyword" value="" onclick="initSearchPopup();">
						</div>
						<!-- <div class="dropMenu">
							<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
							<ul>
								<li class=""><button type="button" id="distributionAddBtn" onclick="initDistributionAddBtn();" data-i18n="menu.deployment">배포</button></li>
								<li class="csv"><button type="button" id="distributionPolicyCsvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
							</ul>
						</div> -->
					</div>
					<div class="tableWrap">
						<table id="distributionTable" class="dataTable checkDataTable">
							<thead>
								<tr>
									<th width="8%" data-i18n="deploymentPolicy.grid.deploymentStatus">배포 상태</th>
									<th width="10%" data-i18n="deploymentPolicy.grid.departmentName">부서명</th>
									<th width="" data-i18n="deploymentPolicy.grid.departmentPath">부서 경로</th>
									<th width="10%" data-i18n="deploymentPolicy.grid.startDate">시작일</th>
									<th width="10%" data-i18n="deploymentPolicy.grid.endDate">종료일</th>
									<th width="10%" data-i18n="deploymentPolicy.grid.computer">컴퓨터</th>
									<th width="10%" data-i18n="deploymentPolicy.grid.latestUpdate">최신 업데이트</th>
								</tr>
							</thead>
							<tbody>
								<tr>
									<td class="result"><span class="return">중지</span></td>
									<td>회사</td>
									<td>회사</td>
									<td>-</td>
									<td>-</td>
									<td>0</td>
									<td>0</td>
								</tr>
								<tr>
									<td class="result"><span class="approval">배포</span></td>
									<td>영업부</td>
									<td>회사/영업부</td>
									<td>2025-01-01</td>
									<td>2025-12-31</td>
									<td>100</td>
									<td>100</td>
								</tr>
								<tr>
									<td class="result"><span class="approval">배포</span></td>
									<td>연구소</td>
									<td>회사/연구소</td>
									<td>2025-01-01</td>
									<td>2025-12-31</td>
									<td>100</td>
									<td>100</td>
								</tr>
							</tbody>
						</table>
					</div>
				</div>
			</div>
		</div>
	</form>
</section>

<!-- 검색 팝업 -->
<section class="popup searchAreaPopup distuserSearchPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="search.searchTit">검색</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="search_wrap">
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.deploymentStatus">배포 상태</label>
							<div class="selectBox">
								<select class="drop" id="searchDeploymentStatus" name="">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="Y" data-i18n="search.option.deployment">배포</option>
									<option value="N" data-i18n="search.option.stop">중지</option>
								</select>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.period">기간</label>
							<div class="selectBox">
								<select class="drop" name="dateSelect" id="searchDateType">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="1" data-i18n="search.option.policyDuration">정책 기간</option>
								</select>
							</div>
							<input type="text" class="datepicker datePickerPopMfile txtBox start-date" name="startDt3" id="startDt3" data-i18n-placeholder="datepicker.startDate" readonly />
							<input type="text" class="datepicker datePickerPopMfile txtBox end-date" name="endDt3" id="endDt3" data-i18n-placeholder="datepicker.endDate" readonly />
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.department">부서</label>
							<div>
								<input type="hidden" id="searchDeptId" name="searchDeptId">
								<input type="text" class="txtBox w100Per" id="deptNameSearch" name="" onkeypress="if(event.keyCode==13) {event.preventDefault(); handleUserSearch();}" />
							</div>
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" onclick="handleUserSearch();" data-i18n="button.search">검색</button>
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.searchAreaPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 배포 등록 팝업 -->
<section class="popup distributionPolicyPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="deploymentPolicy.distributionPolicyPopup.deploymentRegistration">배포 등록</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap mb00">
					<li class="line">
						<div class="item">
							<em class="em100Per"><input type="checkbox" id="releaseYn" name="releaseYn" value=""><span data-i18n="deploymentPolicy.distributionPolicyPopup.deployment">배포</span></em>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="deploymentPolicy.distributionPolicyPopup.selectDepartment">선택 부서</em>
							<div class="flexWrap">
								<input type="hidden" id="deptId" name="deptId">
								<input type="text" class="txtBox w200" id="deptName" name="" value="" id="distributionDeptPath" readonly>
								<div class="checkWrap">
									<input type="checkbox" name="includeSubDept" id="includeSubDept" class="mr10">
									<label data-i18n="search.includeSub">하위부서 포함</label>
								</div>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="deploymentPolicy.distributionPolicyPopup.duration">기간</em>
							<div class="flexWrap sbStart">
								<div class="selectBox w200">
									<select class="drop" name="userRegistrationPeriod" id="SetDeploymentPeriod">
										<option value="1" data-i18n="deploymentPolicy.distributionPolicyPopup.specifyPeriod">기간 지정</option>
										<option value="0" data-i18n="deploymentPolicy.distributionPolicyPopup.unlimited">무기한</option>
									</select>
								</div>
								<input type="text" class="datepicker datePickerPopMfile txtBox start-date" name="startDt4" id="startDt4" data-i18n-placeholder="datepicker.startDate" readonly />
								<input type="text" class="datepicker datePickerPopMfile txtBox end-date" name="endDt4" id="endDt4" data-i18n-placeholder="datepicker.endDate" readonly />
							</div>
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" data-i18n="button.save" onclick="handleDistributionSave();">저장</button>
					<button class="lineBtn" type="button" data-i18n="button.close" onclick="$('.distributionPolicyPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<script type="text/javascript">
	var imgpath = "<c:url value='/images/egovframework/com/cmm/utl/'/>";
</script>

<!-- 부서 트리 관련 스크립트 -->
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/pol/distributionPolicy.js'/>"></script>