import React from "react";
import ReactDOM from "react-dom/client";
import App from "./App";
import { FirestoreProvider } from "./contexts/Firestore";

const root = ReactDOM.createRoot(document.getElementById("root"));

root.render(
	<React.StrictMode>
		<FirestoreProvider>
			<App />
		</FirestoreProvider>
	</React.StrictMode>
);
