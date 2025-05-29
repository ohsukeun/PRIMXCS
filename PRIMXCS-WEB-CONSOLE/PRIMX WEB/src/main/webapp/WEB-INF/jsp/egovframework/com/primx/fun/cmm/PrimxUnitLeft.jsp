<%@ page pageEncoding="utf-8"%>

<section class="left">
	<nav>
		<ul class="firstDepthMenu">
<!-- 			<li class="onlyDepth"> -->
<%-- 				<p class="systemInfo tooltip" data-i18n-tooltip="navigation.systemInfo"><a href="<c:url value='/primx/fun/usr/system/view.do' />" class="menu-link"><span data-i18n="navigation.systemInfo">시스템 정보</span></a></p> --%>
<!-- 			</li> -->
			<!-- <li class="select open"> -->
			<li>
				<p class="approval tooltip" data-i18n-tooltip="navigation.request"><span data-i18n="navigation.request">신청 및 결재</span></p>
				<ul class="secondDepthMenu">
					<li class="select createRequestLi"><a href="<c:url value='/primx/fun/rqt/createRequest/view.do' />" class="menu-link" data-i18n="navigation.createRequest">신청</a></li>
					<li class="approvalLi"><a href="<c:url value='/primx/fun/rqt/requestApproval/view.do' />" class="menu-link" data-i18n="navigation.approval">결재</a></li>
				</ul>
			</li>
			<li>
				<p class="current tooltip" data-i18n-tooltip="navigation.status"><span data-i18n="navigation.status">현황</span></p>
				<ul class="secondDepthMenu">
					<li class="systemStatusLi"><a href="<c:url value='/primx/fun/sys/systemStatus/view.do' />" class="menu-link" data-i18n="navigation.systemStatus">시스템 현황</a></li>
					<li class="moveFileStatusLi"><a href="<c:url value='/primx/fun/file/view.do' />" class="menu-link" data-i18n="navigation.moveFileStatus">이동파일 현황</a></li>
				</ul>
			</li>
			<li class="">
				<p class="history tooltip" data-i18n-tooltip="navigation.history"><span data-i18n="navigation.history">이력</span></p>
				<ul class="secondDepthMenu">
					<li class="productHistoryLi"><a href="<c:url value='/primx/fun/log/productLog/view.do' />" class="menu-link" data-i18n="navigation.productHistory">제품 이력</a></li>
					<li class="operationHistoryLi"><a href="<c:url value='/primx/fun/log/manageLog/view.do' />" class="menu-link" data-i18n="navigation.operationHistory">운영 이력</a></li>
				</ul>
			</li>
			<li> 
				<p class="policy tooltip" data-i18n-tooltip="navigation.policy"><span data-i18n="navigation.policy">정책</span></p>
				<ul class="secondDepthMenu">
					<li class="productPolicyLi"><a href="<c:url value='/primx/fun/pol/product/view.do' />" class="menu-link" data-i18n="navigation.productPolicy">제품 정책</a></li>
					<li><a href="<c:url value='/primx/fun/pol/agent/policy/view.do' />" class="menu-link" data-i18n="navigation.agentPolicy">Agent정책</a></li>
					<li><a href="<c:url value='/primx/fun/pol/distribute/view.do' />" class="menu-link" data-i18n="navigation.deploymentPolicy">배포 정책</a></li>
				</ul>
			</li>
			<li class="">
				<p class="setting tooltip" data-i18n-tooltip="navigation.setting"><span data-i18n="navigation.setting">설정</span></p>
				<ul class="secondDepthMenu">
					<li><a href="<c:url value='/primx/fun/sys/dptUserSetting/view.do?' />" class="menu-link" data-i18n="navigation.departmentAndUserSettings">부서 및 사용자 설정</a></li>
					<li><a href="<c:url value='/primx/fun/sys/update/view.do' />" class="menu-link" data-i18n="navigation.update">업데이트</a></li>
					<li><a href="<c:url value='/primx/fun/sys/product/policySettings/view.do' />" class="menu-link" data-i18n="navigation.productDefaultPolicySettings">제품 기본 정책 설정</a></li>
					<li><a href="<c:url value='/primx/fun/sys/createRequestSetting/view.do' />" class="menu-link" data-i18n="navigation.requestAndApprovalSettings">신청 및 결재 설정</a></li>
					<li><a href="<c:url value='/primx/fun/sys/fileTransferZoneSetting/view.do' />" class="menu-link" data-i18n="navigation.fileTransferZoneSettings">파일전송 Zone 설정</a></li>
					<li><a href="<c:url value='/primx/fun/sys/fileMovePolicySetting/view.do' />" class="menu-link" data-i18n="navigation.moveFilePolicySettings">이동파일 정책 설정</a></li>
				</ul>
			</li>
			<%-- <li class="onlyDepth">
				<p class="dashboard tooltip" data-i18n-tooltip="navigation.dashboard"><a href="<c:url value='/primx/fun/sys/dashboard/view.do' />" class="menu-link"><span data-i18n="navigation.dashboard">대시보드</span></a></p>
			</li> --%>
		</ul>
	</nav>
</section>