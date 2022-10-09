import { createContext, useEffect, useState } from "react";
import { initializeApp } from "firebase/app";
import {
	getFirestore,
	onSnapshot,
	collection,
	query,
} from "firebase/firestore";

const FirestoreContext = createContext([]);

const firebaseConfig = {
	apiKey: "AIzaSyA8NjiwZ3xOS-mTAzinNQYyt28yE7zebxU",
	authDomain: "bike-bois.firebaseapp.com",
	projectId: "bike-bois",
	storageBucket: "bike-bois.appspot.com",
	messagingSenderId: "684456790161",
	appId: "1:684456790161:web:68446794025630baca1730",
	measurementId: "G-FSW5KZ5Y2F",
};

const app = initializeApp(firebaseConfig);

const db = getFirestore(app);

const FirestoreProvider = ({ children }) => {
	const [bikes, setBikes] = useState([]);

	useEffect(() => {
		const bikesQuery = query(collection(db, "bikes"));

		const unsubscribe = onSnapshot(bikesQuery, (querySnapshot) => {
			const bikesArray = [];
			querySnapshot.forEach((doc) => bikesArray.push(doc.data()));
			setBikes(bikesArray);
		});

		return () => unsubscribe();
	}, []);

	const firebaseData = {
		bikes: bikes,
	};

	return (
		<FirestoreContext.Provider value={{ firebaseData }}>
			{children}
		</FirestoreContext.Provider>
	);
};

export { FirestoreContext, FirestoreProvider };
