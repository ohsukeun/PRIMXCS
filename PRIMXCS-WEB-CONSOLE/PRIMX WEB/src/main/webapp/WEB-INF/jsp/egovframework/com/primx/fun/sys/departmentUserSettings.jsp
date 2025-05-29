<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.departmentAndUserSettings">PRIMX | 부서 및 사용자 설정</title>
<form class="departmentUserSettings" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.departmentAndUserSettings"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.setting"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.departmentAndUserSettings"></span>
			</h2>
		</div>
	</div>
	<div class="departUserSection">
		<div class="departmentArea">
			<div class="searchInnerWrap_p mb15">
				<div class="searchBox">
					<input type="text" class="txtBox searchInput2" readonly data-i18n-placeholder="search.departmentSearch">
				</div>
				<div class="dropMenu">
					<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
					<ul>
						<li class="companyAddBtn"><button type="button" onclick="openCompanyAddPopup();" data-i18n="menu.registerComp">회사 등록</button></li>
						<li class="companyCodeListBtn"><button type="button" onclick="openCompanyCodeListPopup();" data-i18n="menu.companyCodeList">회사 코드 조회</button></li>
						<li class="departAddBtn"><button type="button" onclick="openDepartAddPopup();" data-i18n="menu.registerDpt">부서 등록</button></li>
						<li class="departModifyBtn"><button type="button" onclick="openDepartModifyPopup();" data-i18n="menu.edit">수정</button></li>
						<li class="departDeleteBtn"><button type="button" onclick="openDepartDeletePopup();" data-i18n="menu.delete">삭제</button></li>
						<li class="csv"><button type="button" id="treeCsvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
					</ul>
				</div>
			</div>
			<div class="departTreeMap" id="deptTreeContainer">
				<!-- 트리가 여기에 렌더링됨 -->
			</div>
		</div>
		<div class="userArea">
			<div class="searchInnerWrap_p mb15">
				<div class="searchBox">
					<input type="text" class="txtBox searchInput2" readonly data-i18n-placeholder="search.userSearch" name="searchKeyword" value="${searchVO.searchKeyword}">
					<div class="checkWrap">
						<input type="checkbox" name="includeSubDept" id="includeSubDept" class="mr10">
						<label data-i18n="search.includeSub">하위부서 포함</label>
					</div>
				</div>
				<div class="dropMenu">
					<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
					<ul>
						<li class="userAddBtn"><button type="button" onclick="openUserAddPopup();" data-i18n="menu.register">등록</button></li>
						<li class="passwordResetBtn"><button type="button" onclick="openResetPasswordPopup()" data-i18n="menu.passwordReset">비밀번호 초기화</button></li>
						<li class="userDeleteBtn"><button type="button" onclick="openUserDeletePopup();" data-i18n="menu.delete">삭제</button></li>
						<li class="csv"><button type="button" id="departmentUserSettingsCsvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
					</ul>
				</div>
			</div>
			<div class="userBottom">
				<div class="tableWrap">
					<table id="userTable" class="dataTable checkDataTable">
						<thead>
							<tr>
								<th width="2%"></th>
								<th width="5%" data-i18n="departmentAndUserSettings.grid.number">번호</th>
								<th width="10%" data-i18n="departmentAndUserSettings.grid.userID">사용자 ID</th>
								<th width="7%" data-i18n="departmentAndUserSettings.grid.userLevel">사용자 레벨</th>
								<th width="8%" data-i18n="departmentAndUserSettings.grid.userName">사용자명</th>
								<th width="20%" data-i18n="departmentAndUserSettings.grid.email">이메일</th>
								<th width="8%" data-i18n="departmentAndUserSettings.grid.departmentName">부서명</th>
								<th width="5%" data-i18n="departmentAndUserSettings.grid.category">구분</th>
								<th width="5%" data-i18n="departmentAndUserSettings.grid.status">상태</th>
								<th width="5%" data-i18n="departmentAndUserSettings.grid.password">비밀번호</th>
								<th width="12%" data-i18n="departmentAndUserSettings.grid.usagePeriod">사용기간</th>
								<th width="12%" data-i18n="departmentAndUserSettings.grid.registrationDate">등록날짜</th>
							</tr>
						</thead>
						<tbody>
							<!-- <tr>
								<td><input type="checkbox" name="chk" ></td>
								<td>1</td>
								<td>kimsojung</td>
								<td>최고 관리자</td>
								<td>김소정</td>
								<td>kimsojung@korea.com</td>
								<td>기술연구소</td>
								<td>연동</td>
								<td>사용</td>
								<td>초기</td>
								<td>무기한</td>
								<td>2024-06-10 10:31:27</td>
							</tr> -->
						</tbody>
					</table>
				</div>
			</div>
		</div>
	</div>
</form>

<!-- 사용자 수정 -->
<section class="popup userModifyPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="departmentAndUserSettings.userModifyPopup.popTitle">사용자 수정</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.userModifyPopup.userID">사용자 ID</em>
							<div class="idCheck">
								<input type="text" class="txtBox" name="userId" disabled>
								<div class="bu_wrap">
									<button disabled class="disable w160" data-i18n="button.duplicateCheck">중복 체크</button>
								</div>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.userModifyPopup.userName">사용자명</em>
							<div class="">
								<input type="text" class="txtBox" name="userName">
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.userModifyPopup.email">이메일</em>
							<div class="idCheck">
								<input type="text" class="txtBox" name="email">
								<div class="bu_wrap">
									<button class="w160" type="button" onclick="checkMail($('.userModifyPopup input[name=\'email\']').val());" data-i18n="button.duplicateCheck">중복 체크</button>
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
									<select class="drop" name="companyId" id="companyId">
										<option value="ROLE_PRIMX_USER">회사A</option>
										<option value="ROLE_PRIMX_USER">회사B</option>
									</select>
								</div>
							</div>
						</div>
					</li> -->
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.userModifyPopup.department">부서</em>
							<div class="idCheck">
								<input type="text" class="txtBox" name="deptName" id="deptName" readonly>
								<input type="hidden" name="companyId" id="updateCompanyId">
								<input type="hidden" name="deptId" id="deptId">
								<input type="hidden" name="isDeptIdValid" id="isDeptIdValid">
								<div class="bu_wrap">
									<button class="departSelectBtn w160" type="button" onclick="openDepartSelectPopup(null, null);" data-i18n="button.selectDepart">부서 선택</button>
								</div>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.userModifyPopup.userLevel">사용자 레벨</em>
							<div class="adminLevel">
								<div class="selectBox">
									<select class="drop" name="userRole" id="userSelectOption">
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
							<em data-i18n="departmentAndUserSettings.userModifyPopup.usagePeriod">사용기간</em>
							<div class="useDate">
								<div class="selectBox">
									<select class="drop" name="userRegistrationPeriod" id="usePeriod">
										<option value="0" data-i18n="departmentAndUserSettings.userModifyPopup.unlimited">무기한</option>
										<option value="1" data-i18n="departmentAndUserSettings.userModifyPopup.setPeriod">기간 지정</option>
									</select>
								</div>
								<input class="datepicker txtBox" name="userRegistrationExpiredDate" data-i18n-placeholder="datepicker.endDate" readonly />
							</div>
						</div>
					</li>
				</ul>
				<div class="flexWrap addIp">
					<div>
						<input type="checkbox" name="ipPlus" id="ipCheckbox" class="ipPlus" onclick="javascript:changeModifyAreaBox(this)">
						<label for="">IP</label>
					</div>
					<div class="bu_wrap">
						<button class="disable" type="button" onclick="addIp();">+</button>
						<button class="disable" type="button" onclick="removeIp();">-</button>
					</div>
				</div>
				<div class="tableWrap ipTable hide">
					<table class="dataTable noOptionDataTable">
						<thead>
							<tr>
								<th width="5%" class="center"></th>
								<th width="85%">IP</th>
							</tr>
						</thead>
						<tbody>
							<!-- <tr>
								<td class="center"><input type="checkbox" name="chk" ></td>
								<td>
									<input type="text" class="txtBox minInput" maxlength="3"><span class="portDot">.</span>
									<input type="text" class="txtBox minInput" maxlength="3"><span class="portDot">.</span>
									<input type="text" class="txtBox minInput" maxlength="3"><span class="portDot">.</span>
									<input type="text" class="txtBox minInput" maxlength="3">
								</td>
							</tr> -->
						</tbody>
					</table>
				</div>
				<div class="bu_wrap">
					<button type="button" class="saveBtn" onclick="saveUserChanges();" data-i18n="button.save">저장</button>
					<button type="button" class="lineBtn" onclick="$('.userModifyPopup').addClass('hide');" data-i18n="button.cancel">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 사용자 등록 -->
<section class="popup userAddPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="departmentAndUserSettings.userAddPopup.popTitle">사용자 등록</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.userAddPopup.userID">사용자 ID</em>
							<div class="idCheck">
								<input type="text" class="txtBox" name="userId" placeholder="">
								<div class="bu_wrap">
									<button type="button" class="idCheckBtn w160" onclick="checkUserId($('.userAddPopup input[name=\'userId\']').val())" data-i18n="button.duplicateCheck">중복 체크</button>
									<input type="hidden" name="idCheckFlag" value="N">
									<input type="hidden" name="checkedUserId" value="N">
								</div>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.userAddPopup.userName">사용자명</em>
							<div class="">
								<input type="text" class="txtBox" name="userName">
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.userAddPopup.email">이메일</em>
							<div class="idCheck">
								<input type="text" class="txtBox" name="email">
								<div class="bu_wrap">
									<button class="w160" type="button" onclick="checkMail($('.userAddPopup input[name=\'email\']').val());" data-i18n="button.duplicateCheck">중복 체크</button>
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
									<select class="drop" name="companyId" id="companyId">
										<option value="ROLE_PRIMX_ADMIN">회사 선택</option>
										<option value="ROLE_PRIMX_USER">회사A</option>
										<option value="ROLE_PRIMX_USER">회사B</option>
									</select>
								</div>
							</div>
						</div>
					</li> -->
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.userAddPopup.department">부서</em>
							<div class="idCheck">
								<input type="text" class="txtBox" name="deptName" id="userAddPopupDeptName" placeholder="" readonly>
								<input type="hidden" name="companyId" id="companyId">
								<input type="hidden" name="deptId">
								<input type="hidden" name="isDeptIdValid">
								<div class="bu_wrap">
									<button class="departSelectBtn w160" type="button" onclick="openDepartSelectPopup(null, null);" data-i18n="button.selectDepart">부서 선택</button>
								</div>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.userAddPopup.userLevel">사용자 레벨</em>
							<div class="adminLevel">
								<div class="selectBox">
									<select class="drop" name="userRole" >
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
							<em data-i18n="departmentAndUserSettings.userAddPopup.usagePeriod">사용기간</em>
							<div class="useDate">
								<div class="selectBox">
									<select class="drop" name="userRegistrationPeriod" id="userRegistrationPeriod">
										<option value="0" data-i18n="departmentAndUserSettings.userAddPopup.unlimited">무기한</option>
										<option value="1" data-i18n="departmentAndUserSettings.userAddPopup.setPeriod">기간 지정</option>
									</select>
								</div>
								<input class="datepicker txtBox" name="userRegistrationExpiredDate" data-i18n-placeholder="datepicker.endDate" readonly id="userRegistrationExpiredDate" />
							</div>
						</div>
					</li>
				</ul>
				<div class="flexWrap addIp">
					<div>
						<input type="checkbox" name="ipPlus"  class="ipPlus" onclick="javascript:changeAddAreaBox(this)">
						<label for="">IP</label>
					</div>
					<div class="bu_wrap">
						<button class="disable" type="button" onclick="addIp();">+</button>
						<button class="disable" type="button" onclick="removeIp();">-</button>
					</div>
				</div>
				<div class="tableWrap ipTable hide">
					<table class="dataTable noOptionDataTable">
						<thead>
							<tr>
								<th width="5%" class="center"></th>
								<th width="85%">IP</th>
							</tr>
						</thead>
						<tbody>
							<!-- <tr>
								<td class="center"><input type="checkbox" name="chk" ></td>
								<td>
									<input type="text" class="txtBox minInput" maxlength="3"><span class="portDot">.</span>
									<input type="text" class="txtBox minInput" maxlength="3"><span class="portDot">.</span>
									<input type="text" class="txtBox minInput" maxlength="3"><span class="portDot">.</span>
									<input type="text" class="txtBox minInput" maxlength="3">
								</td>
							</tr> -->
						</tbody>
					</table>
				</div>
				<div class="bu_wrap">
					<button type="button" class="saveBtn" onclick="saveUserAdd();" data-i18n="button.save">저장</button>
					<button type="button" class="lineBtn" onclick="$('.userAddPopup').addClass('hide');" data-i18n="button.cancel">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 회사 등록 -->
<section class="popup companyAddPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="departmentAndUserSettings.companyAddPopup.popTitle">회사 등록</strong>
		<form>
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.companyAddPopup.companyName">회사명</em>
							<div>
								<input type="text" class="txtBox" name="" id="companyName">
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.companyAddPopup.companyCode">회사 코드</em>
							<div class="idCheck">
								<input type="text" class="txtBox" name="" id="companyCode">
								<input type="hidden" class="txtBox" name="companyCode" id="checkedCompanyCode">
								<div class="bu_wrap">
                                    <button class="departSelectBtn w160" type="button" data-i18n="button.duplicateCheck" onclick="checkCompanyCode()">중복 체크</button>
                                </div>
							</div>
						</div>

					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" class="saveBtn" onclick="saveCompany();" data-i18n="button.register">등록</button>
					<button type="button" class="lineBtn cancelBtn" onclick="$('.companyAddPopup').addClass('hide');" data-i18n="button.cancel">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 회사 코드 조회 -->
<section class="popup companyCodeListPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="departmentAndUserSettings.companyCodeListPopup.popTitle">회사 코드 조회</strong>
		<form>
			<div class="pop_cont">
				<div class="tableWrap">
					<table id="companyCodeListDatatable" class="dataTable">
						<thead>
							<tr>
								<th data-i18n="departmentAndUserSettings.companyCodeListPopup.grid.companyName">회사명</th>
								<th data-i18n="departmentAndUserSettings.companyCodeListPopup.grid.companyCode">회사 코드</th>
							</tr>
						</thead>
						<tbody>
							<!-- 데이터 테이블이 동적으로 채워질 예정 -->
						</tbody>
					</table>
				</div>
				<div class="bu_wrap">
					<button type="button" class="lineBtn cancelBtn" onclick="$('.companyCodeListPopup').addClass('hide');" data-i18n="button.close">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 부서 등록 -->
<section class="popup departAddPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="departmentAndUserSettings.departAddPopup.popTitle">부서 등록</strong>
		<form>
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.departAddPopup.parentDepartment">상위 부서</em>
							<div class="idCheck">
								<input type="text" class="txtBox" name="deptName" id="departAddPopupDeptName" readonly>
								<input type="hidden" name="deptId">
								<input type="hidden" name="companyId">
								<div class="bu_wrap">
                                    <button class="departSelectBtn w160" type="button" onclick="openDepartSelectPopup($('#departAddPopupDeptName').val(), $('#companyId').val());" data-i18n="button.selectDepart">부서 선택</button>
                                </div>
							</div>
						</div>

					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.departAddPopup.departmentName">부서 이름</em>
							<div>
								<input type="text" class="txtBox" name="chieldDeptName">
							</div>
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" class="saveBtn" onclick="saveDepartment();" data-i18n="button.register">등록</button>
					<button type="button" class="lineBtn cancelBtn" onclick="$('.departAddPopup').addClass('hide');" data-i18n="button.cancel">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 부서 수정 -->
<section class="popup departModifyPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="departmentAndUserSettings.departModifyPopup.popTitle">부서 수정</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line">
						<div class="item">
							<em data-i18n="departmentAndUserSettings.departModifyPopup.departmentName">부서 이름</em>
							<div class="idCheck">
								<input type="text" name="deptName" class="txtBox">
								<input type="hidden" name="deptId">
							</div>
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" class="saveBtn" onclick="modifyDepartment();" data-i18n="button.save">저장</button>
					<button type="button" class="lineBtn" onclick="$('.departModifyPopup').addClass('hide');" data-i18n="button.cancel">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 부서 삭제 -->
<section class="popup departDeletePopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="departmentAndUserSettings.departDeletePopup.popTitle">부서 삭제</strong>
		<form action="">
			<div class="pop_cont">
				<span class="departNameSpan"></span>
				<br>
				<span data-i18n="departmentAndUserSettings.departDeletePopup.confirmDeleteDepartment">부서를 삭제 하시겠습니까?</span>
				<div class="bu_wrap">
					<button type="button" class="confirmBtn" onclick="deleteDepartment();" data-i18n="button.confirm">확인</button>
					<button type="button" class="lineBtn" onclick="$('.departDeletePopup').addClass('hide');" data-i18n="button.cancel">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 사용자 삭제 -->
<section class="popup userDeletePopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="departmentAndUserSettings.userDeletePopup.popTitle">사용자 삭제</strong>
		<form action="">
			<div class="pop_cont">
				<span class="userNameSpan" id="userNameToDelete"></span>
				<br>
				<span data-i18n="departmentAndUserSettings.userDeletePopup.confirmDeleteUser">사용자를 삭제 하시겠습니까?</span>
				<div class="bu_wrap">
					<button type="button" class="confirmBtn" onclick="deleteUser();" data-i18n="button.confirm">확인</button>
					<button type="button" class="lineBtn cancelBtn" onclick="$('.userDeletePopup').addClass('hide');" data-i18n="button.cancel">취소</button>
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

<!-- 검색 팝업(부서) -->
<section class="popup searchAreaPopup departSearchPopup2 hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="search.searchTit">검색</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="search_wrap">
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.department">부서</label>
							<div>
								<div class="flexWrap sbStart">
									<input type="text" class="txtBox w100Per" name="searchKeyword" id="deptNameSearch" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchDepartmentUser('departSearchPopup2'); $('.departSearchPopup2').addClass('hide');}">
								</div>
							</div>
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" onclick="searchDepartmentUser('departSearchPopup2'); $('.departSearchPopup2').addClass('hide');" data-i18n="button.search">검색</button>
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.searchAreaPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 검색 팝업(사용자) -->
<section class="popup searchAreaPopup userSearchPopup2 hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="search.searchTit">검색</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="search_wrap">
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.user">사용자</label>
							<div class="selectBox">
								<select class="drop" name="userType">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="admin" data-i18n="search.option.admin">관리자</option>
									<option value="user" data-i18n="search.option.user">사용자</option>
								</select>
							</div>
							<input type="text" id="selectedUserName" class="txtBox w330" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchDepartmentUser('userSearchPopup2'); $('.userSearchPopup2').addClass('hide');}">
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.type">구분</label>
							<div class="selectBox">
								<select class="drop" name="syncType">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="Y" data-i18n="search.option.integration">연동</option>
									<option value="N" data-i18n="search.option.manual">수동</option>
								</select>
							</div>
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" onclick="searchDepartmentUser('userSearchPopup2');" data-i18n="button.search">검색</button>
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.searchAreaPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 비밀번호 초기화 -->
<section class="popup passwordResetPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="departmentAndUserSettings.passwordResetPopup.popTitle">비밀번호 초기화</strong>
		<form action="">
			<div class="pop_cont" id="userNameToDelete">
				<span data-i18n="departmentAndUserSettings.passwordResetPopup.confirmPasswordReset">비밀번호를 초기화 하시겠습니까?<br></span>
				<div class="bu_wrap">
					<button type="button" class="confirmBtn" onclick="resetPassword();" data-i18n="button.confirm">확인</button>
					<button type="button" class="lineBtn cancelBtn" onclick="$('.passwordResetPopup').addClass('hide');" data-i18n="button.cancel">취소</button>
				</div>
			</div>
		</form>
		<button type="button" class="bu_close_pop"></button>
	</div>
</section>

<script type="text/javascript">
	var imgpath = "<c:url value='/images/egovframework/com/cmm/utl/'/>";
</script>

<!-- 부서 트리 관련 스크립트 -->

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/cmm/customTree.js'/>"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/sys/departmentUserSettings.js'/>"></script>
