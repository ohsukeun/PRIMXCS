<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.update">PRIMX | 업데이트</title>
<form class="update" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.update"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.setting"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.update"></span>
			</h2>
		</div>
	</div>
	<div class="contDiv">
		<h3 class="contTitle" data-i18n="update.clientProgramList">Client 프로그램 목록</h3>
		<div class="tableWrap mb20">
			<table class="dataTable noOptionNoCheckDataTable " id="clientProgramTable">
				<thead>
					<tr>
						<th width="3%" data-i18n="update.grid.number">번호</th>
						<th width="10%" data-i18n="update.grid.program">프로그램</th>
						<th width="10%" data-i18n="update.grid.osVersion">OS버전</th>
						<th data-i18n="update.grid.fileName">파일명</th>
						<th width="15%" data-i18n="update.grid.programVersion">프로그램 버전</th>
						<th width="15%" data-i18n="update.grid.registrationDateTime">등록일시</th>
						<th width="4%" data-i18n="update.grid.upload">업로드</th>
					</tr>
				</thead>
				<tbody>
					<!-- <tr>
						<td>1</td>
						<td>Agent</td>
						<td>64Bit</td>
						<td>파일명</td>
						<td>1.2.7</td>
						<td>2024-10-13 18:09:37</td>
						<td>
							<input class="txtBox" type="file" placeholder="파일 업로드" >
							<div class="bu_wrap mr10">
								<button class="" type="button" onclick="addFileDownloadServer()" data-i18n="button.uploadFile">파일 업로드</button>
							</div>
						</td>
					</tr>
					<tr>
						<td>2</td>
						<td>Agent</td>
						<td>32Bit</td>
						<td>파일명</td>
						<td>1.2.7</td>
						<td>2024-10-13 18:09:37</td>
						<td>
							<div class="inputFileWrap">
								<input class="inputFile" id="upload2" type="file" multiple="">
								<span class="spanFile">파일 업로드</span>
								<label class="labelFile" for="upload2">파일선택</label>
							</div>
						</td>
					</tr>
					<tr>
						<td>3</td>
						<td>ZONECENTRAL</td>
						<td>64Bit</td>
						<td>파일명</td>
						<td>1.2.7</td>
						<td>2024-10-13 18:09:37</td>
						<td>
							<div class="inputFileWrap">
								<input class="inputFile" id="upload3" type="file" multiple="">
								<span class="spanFile">파일 업로드</span>
								<label class="labelFile" for="upload3">파일선택</label>
							</div>
						</td>
					</tr>
					<tr>
						<td>4</td>
						<td>ZONECENTRAL</td>
						<td>32Bit</td>
						<td>파일명</td>
						<td>1.2.7</td>
						<td>2024-10-13 18:09:37</td>
						<td>
							<div class="inputFileWrap">
								<input class="inputFile" id="upload4" type="file" multiple="">
								<span class="spanFile">파일 업로드</span>
								<label class="labelFile" for="upload4">파일선택</label>
							</div>
						</td>
					</tr> -->
				</tbody>
			</table>
		</div>
	</div>
	<div class="contDiv">
		<h3 class="contTitle" data-i18n="update.serverProgramList">Server 프로그램 목록</h3>
		<div class="tableWrap mb20">
			<table class="dataTable noOptionNoCheckDataTable" id="serverProgramTable">
				<thead>
					<tr>
						<th width="3%" data-i18n="update.grid.number">번호</th>
						<th width="35%" data-i18n="update.grid.program">프로그램</th>
						<th width="27%" data-i18n="update.grid.programVersion">프로그램 버전</th>
						<th width="35%" data-i18n="update.grid.installationDate">설치일</th>
					</tr>
				</thead>
				<tbody>
					<!-- <tr>
						<td>4</td>
						<td>관리자 콘솔</td>
						<td>1.2.7</td>
						<td>2024-10-13 18:09:37</td>
					</tr>
					<tr>
						<td>2</td>
						<td>사용자 콘솔</td>
						<td>1.2.7</td>
						<td>2024-10-13 18:09:37</td>
					</tr>
					<tr>
						<td>3</td>
						<td>App Service</td>
						<td>1.2.7</td>
						<td>2024-10-13 18:09:37</td>
					</tr>
					<tr>
						<td>4</td>
						<td>PRIMXDB</td>
						<td>1.2.7</td>
						<td>2024-10-13 18:09:37</td>
					</tr> -->
				</tbody>
			</table>
		</div>
	</div>
	<div class="contDiv">
		<h3 class="contTitle" data-i18n="update.fileDownloadServer">파일 다운로드 서버</h3>
		<div class="tableWrap">
			<table class="dataTable noOptionNoCheckDataTable" id="fileDownloadServerTable">
				<thead>
					<tr>
						<th width="3%" data-i18n="update.grid.number">번호</th>
						<th width="35%" data-i18n="update.grid.serverName">서버명</th>
						<th width="59%" data-i18n="update.grid.address">주소</th>
						<th width="3%">
							<div class="bu_wrap mr10">
								<button class="" type="button" onclick="addFileDownloadServer()">+</button>
							</div>
						</th>
					</tr>
				</thead>
				<tbody>
					<!-- <tr>
						<td>1</td>
						<td><input type="text" class="txtBox w100Per" value="server01"></td>
						<td><input type="text" class="txtBox urlInput w100Per" value="http://192.168.0.23/download"></td>
						<td>
							<div class="bu_wrap mr10">
								<button class="" type="button" onclick="deleteFileDownloadServer(event)">-</button>
							</div>
						</td>
					</tr> -->
				</tbody>
			</table>
		</div>
	</div>
	<div class="bu_wrap">
		<button type="button" onclick="saveFileDownloadServer()" data-i18n="button.save">저장</button>
	</div>

	<!-- 프로그램 업로드 팝업 -->
	<section class="popup clientUploadPopup hide">
		<div class="bgscreen"></div>
		<div class="pop_wrap">
			<strong class="pop_head" data-i18n="update.clientUploadPopup.popTitle">프로그램 업로드</strong>
			<form action="">
				<div class="pop_cont">
					<ul class="setWrap mb00">
						<input type="hidden" class="hide" name="rowId" id="rowId" value="">
						<input type="hidden" class="hide" name="seqNo" id="seqNo" value="">
						<li class="line">
							<div class="item">
								<em data-i18n="update.clientUploadPopup.target">대상</em>
								<div class=""><input type="text" name="program" class="txtBox w100Per" value="" readonly disabled></div>
							</div>
						</li>
						<li class="line">
							<div class="item">
								<em data-i18n="update.clientUploadPopup.osVersion">OS 버전</em>
								<div class=""><input type="text" name="osVersion" class="txtBox w100Per" value="" readonly disabled></div>
							</div>
						</li>
						<li class="line">
							<div class="item">
								<em data-i18n="update.clientUploadPopup.file">파일</em>
								<div class="">
									<div class="inputFileWrap">
										<input class="inputFile" id="upload1" type="file" multiple="">
										<span class="spanFile" data-i18n="button.uploadFile">파일 업로드</span>
										<label class="labelFile w150" for="upload1" data-i18n="button.fileSelection">파일선택</label>
									</div>
								</div>
							</div>
						</li>
						<li class="line">
							<div class="item">
								<em data-i18n="update.clientUploadPopup.version">버전</em>
								<div class=""><input type="text" name="productVersion" class="txtBox w100Per" value=""></div>
							</div>
						</li>
					</ul>
					<div class="bu_wrap">
						<button type="button" class="" data-i18n="button.confirm" onclick="saveClientProgram()">확인</button>
						<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="closeClientUploadPopup()">취소</button>
					</div>
				</div>
			</form>
			<button type="button" class="bu_close_pop"></button>
		</div>
	</section>
</form>

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/sys/update.js' />"></script>